#ifndef CAN_H
#define CAN_H

#include <linux/can.h>

#ifndef CAN_IF_NAME
#define CAN_IF_NAME "can0"
#endif

// Initialize CAN socket (returns socket fd or -1 on error)
int init_can_socket(const char *ifname);

// Send a CAN frame (returns 0 on success, -1 on error)
int send_can_frame(int socket, struct can_frame *frame);

#endif
