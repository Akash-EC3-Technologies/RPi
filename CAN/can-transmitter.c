#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <time.h>

int main()
{
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    int nbytes;

    srand(time(NULL)); // Seed random generator

    // Create CAN socket
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("Socket creation failed");
        return 1;
    }

    // Get interface index
    strcpy(ifr.ifr_name, "can0");
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0)
    {
        perror("ioctl failed");
        close(s);
        return 1;
    }

    // Bind socket to CAN interface
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)))
    {
        perror("Bind failed");
        close(s);
        return 1;
    }

    printf("Transmitting random CAN frames to can0...\nPress CTRL+C to exit\n");

    while (1)
    {
        // Generate random CAN ID (11-bit)
        frame.can_id = rand() & 0x7FF;

        // Generate random data length (0-8 bytes)
        frame.can_dlc = rand() % 9;

        // Fill with random data
        for (int i = 0; i < frame.can_dlc; i++)
        {
            frame.data[i] = rand() & 0xFF;
        }

        // Send frame
        nbytes = write(s, &frame, sizeof(struct can_frame));
        if (nbytes != sizeof(struct can_frame))
        {
            perror("Frame send error");
            break;
        }

        // Display frame details
        printf("Sent: ID=%03X DLC=%d Data=", frame.can_id, frame.can_dlc);
        for (int i = 0; i < frame.can_dlc; i++)
        {
            printf("%02X ", frame.data[i]);
        }
        printf("\n");

        // Random delay (0.5-1.5 seconds)
        usleep(500000 + (rand() % 1000000));
    }

    close(s);
    return 0;
}
