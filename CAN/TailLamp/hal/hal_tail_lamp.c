#include "hal_tail_lamp.h"
#include "../lib/gpio/gpio.h"

static struct gpio_handle break_pedal_handle;

void hal_tail_lamp_init(int pin)
{
    gpio_init(&break_pedal_handle, "/dev/gpiochip0", pin, GPIO_OUTPUT);
}

void hal_tail_lamp_set_state(tail_lamp_state_t state)
{
    gpio_set(&break_pedal_handle, state);
}

tail_lamp_state_t hal_tail_lamp_get_state(void)
{
    return gpio_get(&break_pedal_handle);
}

void hal_tail_lamp_cleanup()
{
    gpio_cleanup(&break_pedal_handle);
}