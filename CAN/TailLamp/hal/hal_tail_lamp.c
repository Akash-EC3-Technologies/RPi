#include "hal_tail_lamp.h"
#include "../lib/gpio/gpio.h"

static struct gpio_handle tail_lamp_handle;

void hal_tail_lamp_init(int pin)
{
    gpio_init(&tail_lamp_handle, "/dev/gpiochip0", pin, GPIO_OUTPUT);
}

void hal_tail_lamp_set_state(tail_lamp_state_t state)
{
    gpio_set(&tail_lamp_handle, state);
}

tail_lamp_state_t hal_tail_lamp_get_state(void)
{
    return gpio_get(&tail_lamp_handle);
}

void hal_tail_lamp_cleanup()
{
    gpio_cleanup(&tail_lamp_handle);
}