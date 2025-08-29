#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../can/can.h"
#include "../../hal/hal_break_pedal.h"
#include "uds.h"

#define RESP_CAN_ID 0x7E8
#define RDBI_SID 0x22 // Read Data By Identifier
#define WDBI_SID 0x2E // Read Data By Identifier

#define POS_RSP_SID(sid) ((sid) + 0x40)
#define NEG_RSP_SID 0x7F // Negative response SID

// Supported DIDs
#define DID_BreakPedalStatus 0xF140

#define MAX_DATA_LEN 4 // For single-frame, max 4 bytes of actual data

// Send a negative response
static void
send_negative_response(int socket, unsigned char response_code)
{
    struct can_frame tx;
    tx.can_id = RESP_CAN_ID;
    tx.can_dlc = 8;
    memset(tx.data, 0, sizeof(tx.data));

    tx.data[0] = 0x03;        // Single Frame, 3 bytes data
    tx.data[1] = NEG_RSP_SID; // Negative Response
    tx.data[2] = RDBI_SID;    // Original SID
    tx.data[3] = response_code;

    send_can_frame(socket, &tx);
}

int set_break_pedal_state(unsigned short did, unsigned char value)
{
    hal_set_break_pedal_state((int)value);
    return 0;
}

// UDS server loop
void start_uds_server(int socket)
{
    struct can_frame rx_frame;
    struct can_frame tx_frame;

    printf("UDS Server started...\n");

    while (1)
    {
        int nbytes = read_can_frame(socket, &rx_frame);

        if (nbytes > 0)
        {
            if (rx_frame.can_dlc >= 4)
            {
                unsigned char pci = rx_frame.data[0];
                unsigned char sid = rx_frame.data[1];
                unsigned short did = (rx_frame.data[2] << 8) | rx_frame.data[3];

                if ((pci & 0xF0) == 0x00) // Frame type is singe frame format
                {
                    switch (sid)
                    {
                    case WDBI_SID:
                        printf("UDS: WDBI request for DID 0x%04X\n", did);
                        if (DID_BreakPedalStatus == did)
                        {
                            if (rx_frame.can_dlc >= 5)
                            {
                                unsigned char value = rx_frame.data[4];
                                int ret = set_break_pedal_state(did, value);

                                memset(&tx_frame, 0, sizeof(tx_frame));
                                tx_frame.can_id = RESP_CAN_ID;
                                tx_frame.can_dlc = 8;

                                if (ret == 0)
                                {
                                    // Positive response: SID + 0x40
                                    tx_frame.data[0] = 0x03;
                                    tx_frame.data[1] = POS_RSP_SID(WDBI_SID); // 0x6E
                                    tx_frame.data[2] = (did >> 8) & 0xFF;
                                    tx_frame.data[3] = did & 0xFF;
                                }
                                else
                                {
                                    // Negative response: NRC = 0x31 (Request Out of Range)
                                    tx_frame.data[0] = 0x03;
                                    tx_frame.data[1] = NEG_RSP_SID;
                                    tx_frame.data[2] = WDBI_SID;
                                    tx_frame.data[3] = 0x31;
                                }

                                if (send_can_frame(socket, &tx_frame) > 0)
                                {
                                    printf("UDS: Sent WDBI response for DID 0x%04X\n", did);
                                }
                                else
                                {
                                    perror("send_can_frame");
                                }
                            }
                        }
                    }
                }
            }
        }
        usleep(500000); // 500ms
    }
}