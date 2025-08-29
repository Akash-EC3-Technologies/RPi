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

int init_can_socket(const char *ifname, canid_t filter_id)
{
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0)
    {
        perror("socket");
        return -1;
    }

    struct ifreq ifr;
    struct sockaddr_can addr;

    // Copy interface name to ifreq structure
    strncpy(ifr.ifr_name, ifname, IFNAMSIZ - 1);
    ifr.ifr_name[IFNAMSIZ - 1] = '\0';

    // Get interface index
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("ioctl");
        close(sock);
        return -1;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    // Apply filter (if filter_id != 0, otherwise accept all frames)
    if (filter_id != 0)
    {
        struct can_filter rfilter;
        rfilter.can_id = filter_id;
        rfilter.can_mask = CAN_SFF_MASK; // Match all bits for standard ID
        if (setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter)) < 0)
        {
            perror("setsockopt");
            close(sock);
            return -1;
        }
    }

    // Bind the socket
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bind");
        close(sock);
        return -1;
    }

    return sock;
}

// Read a CAN frame (non-blocking)
int read_can_frame(int socket, struct can_frame *frame)
{
    int nbytes = read(socket, frame, sizeof(struct can_frame));
    if (nbytes < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            return 0; // No data available yet
        }
        else
        {
            perror("Read");
            return -1;
        }
    }
    return nbytes; // Frame read successfully
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
