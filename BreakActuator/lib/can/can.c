#include "can.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can/raw.h>
#include <fcntl.h>
#include <errno.h>

// Initialize CAN socket
int init_can_socket(const char *ifname)
{
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;

    if ((s = socket(PF_CAN, SOCK_RAW | SOCK_NONBLOCK, CAN_RAW)) < 0)
    {
        perror("Socket");
        return -1;
    }

    strcpy(ifr.ifr_name, ifname);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("ioctl");
        close(s);
        return -1;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("Bind");
        close(s);
        return -1;
    }

    return s;
}

// Send a CAN frame
int send_can_frame(int socket, struct can_frame *frame)
{
    int nbytes = write(socket, frame, sizeof(struct can_frame));
    if (nbytes != sizeof(struct can_frame))
    {
        perror("Write");
        return -1;
    }
    return nbytes;
}
