#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    int value;
} mock_internal;

int gpio_init(gpio_handle *handle, const char *chipname, unsigned int line_num, gpio_direction dir)
{
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
    mock_internal *internal = handle->internal;
    internal->value = value;
    return 0;
}

int gpio_get(gpio_handle *handle)
{
    mock_internal *internal = handle->internal;
    return internal->value;
}

void gpio_cleanup(gpio_handle *handle)
{
    free(handle->internal);
    handle->internal = NULL;
}
