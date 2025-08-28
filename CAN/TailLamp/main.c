#include <stdio.h>
#include <unistd.h>
#include "lib/can/can.h"
#include "hal/hal_tail_lamp.h"

#define BRAKE_MSG_ID 0x123
#define TAIL_LAMP_PIN 17

void process_can_message(struct can_frame frame)
{
    if (frame.can_id == BRAKE_MSG_ID && frame.can_dlc == 1)
    {
        if (frame.data[0] == 0x01)
        {
            hal_tail_lamp_set_state(TAIL_LAMP_ON);
            printf("tail lamp state is: %s\n", hal_tail_lamp_get_state() == TAIL_LAMP_ON ? "ON" : "OFF");
        }
        else if (frame.data[0] == 0x00)
        {
            hal_tail_lamp_set_state(TAIL_LAMP_OFF);
            printf("tail lamp state is: %s\n", hal_tail_lamp_get_state() == TAIL_LAMP_ON ? "ON" : "OFF");
        }
    }
}

int main()
{
    int sock = init_can_socket(CAN_IF_NAME);
    if (sock < 0)
    {
        return 1;
    }
    hal_tail_lamp_init(TAIL_LAMP_PIN);
    // Try reading a frame in non-blocking mode
    struct can_frame rx_frame;
    while (1)
    {
        if (read_can_frame(sock, &rx_frame))
        {
            process_can_message(rx_frame);
        }
        else
        {
            usleep(500000); // 500 ms delay
        }
    }

    close(sock);
    hal_tail_lamp_cleanup();
    return 0;
}
