#ifndef HAL_TAIL_LAMP_H
#define HAL_TAIL_LAMP_H

typedef enum
{
    TAIL_LAMP_OFF = 0,
    TAIL_LAMP_ON = 1
} tail_lamp_state_t;

void hal_tail_lamp_init(int pin);
void hal_tail_lamp_set_state(tail_lamp_state_t state);
tail_lamp_state_t hal_tail_lamp_get_state(void);
void hal_tail_lamp_cleanup();

#endif
