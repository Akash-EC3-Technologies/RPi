#ifndef GPIO_H
#define GPIO_H

// Initialize a GPIO pin as output
void gpio_init(int pin);

// Set GPIO pin value (0 = LOW, 1 = HIGH)
void gpio_set(int pin, int value);

// Read GPIO pin value (0 = LOW, 1 = HIGH) – works only if pin is INPUT
int gpio_read(int pin);

// Release the GPIO pin
void gpio_cleanup(int pin);

#endif // GPIO_H
