#include <stdio.h>
#include <unistd.h>
#include "lib/can/can.h"
#include "hal/hal_break_pedal.h"

#define BREAK_PEDAl_PIN 17

#define BRAKE_MSG_ID 0x123

void send_can_message(int sock, break_pedal_state_t state)
{
    struct can_frame frame;
    frame.can_id = BRAKE_MSG_ID;
    frame.can_dlc = 1;
    frame.data[0] = (state == BREAK_PEDAL_ACTIVE) ? 1 : 0;
    send_can_frame(sock, &frame);
}

int main()
{
    int sock = init_can_socket(CAN_IF_NAME);
    if (sock < 0)
    {
        return 1;
    }
    hal_break_pedal_init(BREAK_PEDAl_PIN);

    while (1)
    {
        if (hal_break_pedal_state_changed())
        {
            send_can_message(sock, hal_read_break_pedal_state());
        }
        else
        {
            usleep(500000); // 500 ms delay
        }
    }

    close(sock);
    hal_break_pedal_cleanup();
    return 0;
}
