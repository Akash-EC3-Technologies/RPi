#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "lib/can/can.h"
#include "hal/hal_tail_lamp.h"
#include "lib/uds/uds.h"

#define BRAKE_MSG_ID 0x123
#define UDS_MSG_ID 0x7E0
#define TAIL_LAMP_PIN 17

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

    struct can_frame rx_frame;
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
    hal_tail_lamp_init(TAIL_LAMP_PIN);

    // Start UDS in a separate thread
    uds_thread = start_uds_thread(uds_socket);

    while (1)
    {
        if (read_can_frame(app_socket, &rx_frame))
        {
            process_can_message(rx_frame);
        }
        else
        {
            usleep(500000); // 500 ms delay
        }
    }

    pthread_join(uds_thread, NULL);
    close(app_socket);
    close(uds_socket);
    hal_tail_lamp_cleanup();
    return 0;
}
