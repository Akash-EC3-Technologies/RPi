#include "hal_break_pedal.h"
#include "../lib/gpio/gpio.h"

static int g_break_pedal_pin = 17;
static break_pedal_state_t current_state;
void hal_break_pedal_init(int pin)
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
    break_pedal_state_t new_state = hal_read_break_pedal_state();
    if (new_state != current_state)
    {
        current_state = new_state;
        return 1; // state changed
    }
    return 0; // state not changed
}

void hal_break_pedal_cleanup()
{
    gpio_cleanup(g_break_pedal_pin);
}