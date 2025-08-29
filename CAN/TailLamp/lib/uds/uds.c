#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../can/can.h"
#include "../../hal/hal_tail_lamp.h"
#include "uds.h"

#define RESP_CAN_ID 0x7E8
#define RDBI_SID 0x22 // Read Data By Identifier
#define POS_RSP_SID(sid) ((sid) + 0x40)
#define NEG_RSP_SID 0x7F // Negative response SID

// Supported DIDs
#define DID_GetTailLampStatus 0xF150

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

int get_tail_lamp_state(unsigned short did, unsigned char *buffer)
{
    buffer[0] = (unsigned char)hal_tail_lamp_get_state();
    return 1;
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

                if ((pci & 0xF0) == 0x00 && sid == RDBI_SID)
                {
                    printf("UDS: RDBI request for DID 0x%04X\n", did);

                    if (DID_GetTailLampStatus == did)
                    {
                        unsigned char data[MAX_DATA_LEN];
                        int len = get_tail_lamp_state(did, data);

                        if (len < 0 || len > MAX_DATA_LEN)
                        {
                            send_negative_response(socket, 0x31); // Request out of range
                        }
                        else
                        {
                            tx_frame.can_id = RESP_CAN_ID;
                            tx_frame.can_dlc = 8;
                            memset(tx_frame.data, 0, sizeof(tx_frame.data));

                            tx_frame.data[0] = 3 + len; // PCI length = SID+DID+data
                            tx_frame.data[1] = POS_RSP_SID(RDBI_SID);
                            tx_frame.data[2] = (did >> 8) & 0xFF;
                            tx_frame.data[3] = did & 0xFF;
                            memcpy(&tx_frame.data[4], data, len);

                            if (send_can_frame(socket, &tx_frame) > 0)
                            {
                                printf("UDS: Sent response for DID 0x%04X\n", did);
                            }
                            else
                            {
                                perror("send_can_frame failed");
                            }
                        }
                    }
                }
            }
        }
    }
    usleep(500000); // 500ms
}
