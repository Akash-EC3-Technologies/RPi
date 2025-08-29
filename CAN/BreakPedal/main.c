#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "lib/can/can.h"
#include "hal/hal_break_pedal.h"
#include "lib/uds/uds.h"

#define BREAK_PEDAl_PIN 27

#define BRAKE_MSG_ID 0x123
#define UDS_MSG_ID 0x7E0

struct uds_thread_args
{
    int socket;
};

void *uds_thread_func(void *arg)
{
    struct uds_thread_args *args = (struct uds_thread_args *)arg;
    start_uds_server(args->socket);
    return NULL;
}

pthread_t start_uds_thread(int uds_socket)
{
    pthread_t uds_thread;
    struct uds_thread_args *thread_args = malloc(sizeof(struct uds_thread_args));
    // Prepare thread args
    thread_args->socket = uds_socket;

    // Start UDS thread
    if (pthread_create(&uds_thread, NULL, uds_thread_func, thread_args) != 0)
    {
        perror("pthread_create");
        return 1;
    }
}

void send_can_message(int sock, break_pedal_state_t state)
{
    struct can_frame frame;
    frame.can_id = BRAKE_MSG_ID;
    frame.can_dlc = 1;
    frame.data[0] = (state == BREAK_PEDAL_ACTIVE) ? 0x01 : 0x00;
    send_can_frame(sock, &frame);
}

int main()
{
    pthread_t uds_thread;
    int app_socket = init_can_socket(CAN_IF_NAME, BRAKE_MSG_ID);
    if (app_socket < 0)
    {
        return 1;
    }
    int uds_socket = init_can_socket(CAN_IF_NAME, UDS_MSG_ID);
    if (uds_socket < 0)
    {
        return 1;
    }
    hal_break_pedal_init(BREAK_PEDAl_PIN);
    // Start UDS in a separate thread
    uds_thread = start_uds_thread(uds_socket);

    while (1)
    {
        if (hal_break_pedal_state_changed())
        {
            printf("break pedal state changed: %s\n", hal_read_break_pedal_state() == BREAK_PEDAL_ACTIVE ? "ACTIVE" : "INACTIVE");
            send_can_message(app_socket, hal_read_break_pedal_state());
        }
        else
        {
            usleep(500000); // 500 ms delay
        }
    }

    pthread_join(uds_thread, NULL);
    close(app_socket);
    close(uds_socket);
    hal_break_pedal_cleanup();
    return 0;
}
