#include "hal_break_pedal.h"
#include "../lib/gpio/gpio.h"

static struct gpio_handle break_pedal_handle;
static break_pedal_state_t current_state;

void hal_break_pedal_init(int pin)
{
    gpio_init(&break_pedal_handle, "/dev/gpiochip0", pin, GPIO_INPUT);
    // store the intial state as current_state
    current_state = hal_read_break_pedal_state();
}

break_pedal_state_t hal_read_break_pedal_state()
{
    return gpio_get(&break_pedal_handle);
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
    gpio_cleanup(&break_pedal_handle);
}