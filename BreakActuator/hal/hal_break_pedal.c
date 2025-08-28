#include "hal_break_pedal.h"
#include "../lib/gpio/gpio.h"

static int g_break_pedal_pin = 17;
static break_pedal_state_t current_state;
int hal_break_pedal_init(int pin)
{
    g_break_pedal_pin = pin;
    gpio_init(g_break_pedal_pin);
    // store the intial state as current_state
    current_state = hal_read_break_pedal_state();
}

break_pedal_state_t hal_read_break_pedal_state()
{
    return gpio_read(g_break_pedal_pin);
}

int hal_break_pedal_state_changed()
{
    current_state =
}

void hal_break_pedal_cleanup()
{
    gpio_cleanup(g_break_pedal_pin);
}