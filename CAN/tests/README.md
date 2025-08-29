# **CAN Integration Test using pytest, python-can, and can-isotp**

This integration test verifies the interaction between two ECUs (Brake Pedal ECU and Tail Lamp ECU) over a virtual CAN interface (`vcan0`). The test uses **ISO-TP (ISO 15765-2)** for diagnostic communication and raw CAN monitoring for verifying ECU behavior.

## **Test Overview**

-   The test uses **UDS WriteDataByIdentifier (WDBI)** and **ReadDataByIdentifier (RDBI)** requests to:

    1. Set the brake pedal status (active/inactive).
    2. Verify that the brake ECU sends a raw CAN message (`0x123`) with the correct status.
    3. Read the tail lamp status and verify that it matches the expected value.

## **Prerequisites**

1. **Python 3.8+**
2. **pip** (Python package manager)
3. Linux environment with `socketcan` support.
4. Virtual CAN interface (`vcan0`) enabled.

## **Setup Instructions**

### **1. Create Python Virtual Environment**

```bash
python3 -m venv venv
source venv/bin/activate
```

### **2. Install Python Dependencies**

Install `pytest`, `python-can`, and `can-isotp`:

```bash
pip3 install pytest python-can can-isotp
```

## **Running the Tests**

### **1. Start the ECUs in Docker Containers**

Ensure both ECU containers (`brake_pedal` and `tail_lamp`) are running and connected to `vcan0`:

```bash
cd ../BreakPedal
make docker-run
cd ../TailLamp
make docker-run
```

### **2. Run the Pytest Integration Test**

From the `tests` directory:

```bash
pytest -v -s integration-test.py
```

-   `-v`: verbose output
-   `-s`: show logs in the console

### **3. Test Parameters**

The test includes two parameterized cases:

-   **active**: Brake active (`0x01`), Tail lamp should be ON (`0x01`)
-   **inactive**: Brake inactive (`0x00`), Tail lamp should be OFF (`0x00`)
