#ifndef GPIO_H
#define GPIO_H

typedef enum
{
    GPIO_OUTPUT,
    GPIO_INPUT
} gpio_direction;

typedef struct gpio_handle
{
    void *internal; // Opaque pointer for real or mock implementation
    gpio_direction direction;
    unsigned int line_num;
} gpio_handle;

// Initialize GPIO pin (input or output)
int gpio_init(gpio_handle *handle, const char *chipname, unsigned int line_num, gpio_direction dir);

// Set GPIO value (only for output)
int gpio_set(gpio_handle *handle, int value);

// Get GPIO value (only for input)
int gpio_get(gpio_handle *handle);

// Cleanup resources
void gpio_cleanup(gpio_handle *handle);

#endif
