#!/bin/bash

# Raspberry Pi 5 Onboard LED Blink Script (Alpine Compatible)
# Note: This script is optimized for running in an Alpine container

# Function to set LED state
set_led() {
    echo $1 > /sys/class/leds/PWR/brightness
}

# Function to clean up on exit
cleanup() {
    set_led 1  # Turn LED back on
    exit 0
}

# Trap exit signals
trap cleanup INT TERM EXIT

# Check LED access
if [ ! -w /sys/class/leds/PWR/brightness ]; then
    echo "ERROR: Cannot access LED control. Make sure to run with:"
    echo "  --privileged and -v /sys/class/leds:/sys/class/leds"
    exit 1
fi

echo "Blinking Raspberry Pi 5 PWR LED..."
echo "Press Ctrl-C to stop"

# Blink loop
while true; do
    set_led 1   # LED on
    sleep 0.5
    set_led 0   # LED off
    sleep 0.5
done