#include "hal_tail_lamp.h"
#include "../lib/gpio/gpio.h"

static int g_tail_lamp_pin = 17;

void hal_tail_lamp_init(int pin)
{
    g_tail_lamp_pin = pin;
    gpio_init(g_tail_lamp_pin);
}

void hal_tail_lamp_set_state(tail_lamp_state_t state)
{
    gpio_set(g_tail_lamp_pin, state);
}

tail_lamp_state_t hal_tail_lamp_get_state(void)
{
    return gpio_read(g_tail_lamp_pin);
}

void hal_tail_lamp_cleanup()
{
    gpio_cleanup(g_tail_lamp_pin);
    g_tail_lamp_pin = 0;
}