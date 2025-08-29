#include "gpio.h"
#include <gpiod.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
    struct gpiod_chip *chip;
    struct gpiod_line *line;
} gpio_internal;

int gpio_init(gpio_handle *handle, const char *chipname, unsigned int line_num, gpio_direction dir)
{
    gpio_internal *internal = malloc(sizeof(gpio_internal));
    if (!internal)
        return -1;

    internal->chip = gpiod_chip_open(chipname);
    if (!internal->chip)
    {
        perror("Failed to open GPIO chip");
        free(internal);
        return -1;
    }

    internal->line = gpiod_chip_get_line(internal->chip, line_num);
    if (!internal->line)
    {
        perror("Failed to get GPIO line");
        gpiod_chip_close(internal->chip);
        free(internal);
        return -1;
    }

    if (dir == GPIO_OUTPUT)
    {
        if (gpiod_line_request_output(internal->line, "gpio_control", 0) < 0)
        {
            perror("Failed to request line as output");
            gpiod_chip_close(internal->chip);
            free(internal);
            return -1;
        }
    }
    else
    {
        if (gpiod_line_request_input(internal->line, "gpio_control") < 0)
        {
            perror("Failed to request line as input");
            gpiod_chip_close(internal->chip);
            free(internal);
            return -1;
        }
    }

    handle->internal = internal;
    handle->direction = dir;
    handle->line_num = line_num;
    return 0;
}

int gpio_set(gpio_handle *handle, int value)
{
    gpio_internal *internal = handle->internal;
    return gpiod_line_set_value(internal->line, value);
}

int gpio_get(gpio_handle *handle)
{
    gpio_internal *internal = handle->internal;
    return gpiod_line_get_value(internal->line);
}

void gpio_cleanup(gpio_handle *handle)
{
    gpio_internal *internal = handle->internal;
    if (internal)
    {
        if (internal->line)
            gpiod_line_release(internal->line);
        if (internal->chip)
            gpiod_chip_close(internal->chip);
        free(internal);
        handle->internal = NULL;
    }
}
