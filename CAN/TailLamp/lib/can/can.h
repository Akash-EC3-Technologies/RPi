#ifndef CAN_H
#define CAN_H

#include <linux/can.h>

#ifndef CAN_IF_NAME
#define CAN_IF_NAME "can0"
#endif

// Initialize CAN socket
// Returns: socket fd or -1 on error
int init_can_socket(const char *ifname, canid_t filter_id);
// Send a CAN frame
// Returns: number of bytes written if success,-1 if error
int send_can_frame(int socket, struct can_frame *frame);

// Read a CAN frame in non-blocking mode
// Returns: number of bytes read if success, 0 if no data, -1 if error
int read_can_frame(int socket, struct can_frame *frame);

#endif
