#ifndef HAL_BREAK_PEADL_H
#define HAL_BREAK_PEADL_H

typedef enum
{
    BREAK_PEDAL_INACTIVE = 0,
    BREAK_PEDAL_ACTIVE = 1
} break_pedal_state_t;

void hal_break_pedal_init(int pin);
break_pedal_state_t hal_read_break_pedal_state();
int hal_break_pedal_state_changed();
void hal_set_break_pedal_state(break_pedal_state_t state);
void hal_break_pedal_cleanup();

#endif
