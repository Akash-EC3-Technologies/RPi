import time
import logging
import pytest
import can
import isotp

CAN_CHANNEL = "vcan0"
TX_ID = 0x7E0
RX_ID = 0x7E8
BREAK_MSG_ID = 0x123
WRITE_DID = (0xF1, 0x40)
READ_DID  = (0xF1, 0x50)
ISOTP_WAIT = 3.0        # increased a bit
RAW_CAN_WAIT = 3.0

log = logging.getLogger("pytest-isotp")

@pytest.fixture
def isotp_stack_and_buses():
    """
    Returns (isotp_stack, isotp_bus, raw_bus)
    - isotp_bus is used by the isotp stack
    - raw_bus is a separate python-can Bus instance used only for raw CAN reception
    """
    # ISO-TP bus (used by the isotp stack)
    isotp_bus = can.interface.Bus(channel=CAN_CHANNEL, interface="socketcan")
    addr = isotp.Address(isotp.AddressingMode.Normal_11bits, rxid=RX_ID, txid=TX_ID)
    params = {"blocking_send": True}
    stack = isotp.CanStack(bus=isotp_bus, address=addr, params=params)

    # Separate raw bus socket for monitoring raw CAN frames (prevents race with isotp stack)
    raw_bus = can.interface.Bus(channel=CAN_CHANNEL, interface="socketcan")

    # start ISO-TP background processing thread
    stack.start()
    log.info("ISO-TP stack started (TX 0x%03X -> RX 0x%03X)", TX_ID, RX_ID)

    yield stack, isotp_bus, raw_bus

    # teardown
    try: stack.stop()
    except Exception: pass
    try: isotp_bus.shutdown()
    except Exception: pass
    try: raw_bus.shutdown()
    except Exception: pass


def wait_for_raw_break(raw_bus, timeout=RAW_CAN_WAIT):
    """Listen on raw_bus for the break message and return first data byte."""
    deadline = time.time() + timeout
    raw_bus.set_filters([{"can_id": BREAK_MSG_ID, "can_mask": 0x7FF}])
    while time.time() < deadline:
        msg = raw_bus.recv(timeout=deadline - time.time())
        if msg is None:
            continue
        if msg.arbitration_id == BREAK_MSG_ID and msg.dlc >= 1:
            log.info("Raw CAN observed id=0x%03X data=%s", msg.arbitration_id, msg.data.hex())
            return msg.data[0]
    raise TimeoutError(f"No raw break CAN message (id 0x{BREAK_MSG_ID:03X}) observed")


@pytest.mark.integration
@pytest.mark.parametrize(
    "case_name, break_status, expected_tail_lamp_status",
    [
        ("active", 0x01, 0x01),
        ("inactive", 0x00, 0x00),
    ],
    ids=["active", "inactive"]
)
def test_end_to_end_break_to_taillamp(isotp_stack_and_buses, case_name, break_status, expected_tail_lamp_status):
    stack, isotp_bus, raw_bus = isotp_stack_and_buses

    # First write the Break Status DID via isotp stack
    payload_write_break_status = bytes([0x2E, WRITE_DID[0], WRITE_DID[1], break_status])
    stack.send(payload_write_break_status)

    # wait for iso-tp response
    deadline = time.time() + ISOTP_WAIT
    resp = None
    while time.time() < deadline:
        try:
            stack.process()
        except Exception:
            pass
        if stack.available():
            resp = stack.recv()
            break
        time.sleep(0.01)
    assert resp is not None, "No ISO-TP response for WDBI"
    assert resp[0] == 0x6e and resp[1] == WRITE_DID[0] and resp[2] == WRITE_DID[1], "Invalid WDBI response"

    # Wait for break status message from the raw can bus
    brk = wait_for_raw_break(raw_bus, timeout=RAW_CAN_WAIT)
    assert brk == break_status, "Invalid break status (expected 0x%02X but got 0x%02X)" % (break_status, brk)

    # Now read Tail Lamp DID via isotp stack
    payload_read_tail_lamp_status = bytes([0x22, READ_DID[0], READ_DID[1]])
    stack.send(payload_read_tail_lamp_status)
    # wait for iso-tp response
    deadline = time.time() + ISOTP_WAIT
    resp = None
    while time.time() < deadline:
        try:
            stack.process()
        except Exception:
            pass
        if stack.available():
            resp = stack.recv()
            break
        time.sleep(0.01)
    assert resp is not None, "No ISO-TP response for RDBI"
    assert resp[0] == 0x62 and resp[1] == READ_DID[0] and resp[2] == READ_DID[1], "Invalid RDBI response"
    assert resp[3] == expected_tail_lamp_status, "Invalid tail lamp status (expected 0x%02X but got 0x%02X)" % (expected_tail_lamp_status, resp[3])

