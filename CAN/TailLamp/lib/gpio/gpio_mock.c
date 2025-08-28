#include "gpio.h"

static int pins[40];

void gpio_init(int pin)
{
    // init given pin as 0
    pins[pin] = 0;
}

void gpio_set(int pin, int value)
{
    pins[pin] = value;
}

int gpio_read(int pin)
{
    return pins[pin];
}

void gpio_cleanup(int pin)
{
    pins[pin] = 0;
}
