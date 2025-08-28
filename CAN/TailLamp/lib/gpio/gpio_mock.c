#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int value;
} mock_internal;

int gpio_init(gpio_handle *handle, const char *chipname, unsigned int line_num, gpio_direction dir)
{
    printf("[MOCK] Init GPIO %u as %s\n", line_num, dir == GPIO_OUTPUT ? "OUTPUT" : "INPUT");
    mock_internal *internal = malloc(sizeof(mock_internal));
    if (!internal)
        return -1;
    internal->value = 0;
    handle->internal = internal;
    handle->direction = dir;
    handle->line_num = line_num;
    return 0;
}

int gpio_set(gpio_handle *handle, int value)
{
    if (handle->direction != GPIO_OUTPUT)
        return -1;
    mock_internal *internal = handle->internal;
    internal->value = value;
    printf("[MOCK] Set GPIO %u to %d\n", handle->line_num, value);
    return 0;
}

int gpio_get(gpio_handle *handle)
{
    if (handle->direction != GPIO_INPUT)
        return -1;
    mock_internal *internal = handle->internal;
    printf("[MOCK] Read GPIO %u -> %d\n", handle->line_num, internal->value);
    return internal->value;
}

void gpio_cleanup(gpio_handle *handle)
{
    printf("[MOCK] Cleanup GPIO %u\n", handle->line_num);
    free(handle->internal);
    handle->internal = NULL;
}
