#!/bin/bash

LED=PWR

# Function to set LED state
set_led_state() {
    echo $1 > /sys/class/leds/$LED/brightness > /dev/null
}

# Function to clean up on exit
cleanup() {
    set_led 1  # Turn LED back on (normal state)
    exit 0
}

# Trap Ctrl-C and other exits
trap cleanup EXIT

# Check if we have permission to control the LED
if [ ! -w /sys/class/leds/$LED/brightness ]; then
    echo "Error: Need root permissions to control the LED."
    echo "Please run this script with sudo."
    exit 1
fi

echo "Blinking Raspberry Pi 5 onboard LED ($LED LED)..."
echo "Press Ctrl-C to stop."

# Blink loop
while true; do
    set_led_state 1   # LED on
    sleep 0.5   # Wait 0.5 seconds
    set_led_state 0   # LED off
    sleep 0.5   # Wait 0.5 seconds
done