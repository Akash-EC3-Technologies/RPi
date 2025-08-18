# Raspberry Pi 5 Accessing the hardware from Docker

This project demonstrates how to control the Hardware on a Raspberry Pi 5 using Docker and a minimal Alpine Linux environment.

## Experiment Description

The goal of this experiment is to:

-   Build a lightweight container image for Raspberry Pi 5.
-   Control the onboard LEDs (`PWR`, `ACT`, etc.) directly via the Linux LED subsystem.
-   Explore running containers with different levels of hardware access.

## Building the Experiment

Build the Docker image:

```bash
docker build -t pi5-led-blink-alpine .
```

## Running the Experiment

### 1. Run with Full Hardware Access

This mode provides the container **privileged access** to the host hardware:

```bash
docker run -it --rm --privileged pi5-led-blink-alpine
```

### 2. Run with Limited LED Access

This mode restricts access to only the **PWR LED**, by mounting just its brightness control path into the container:

```bash
docker run --rm \
  -v /sys/class/leds/PWR/brightness:/sys/class/leds/PWR/brightness:rw \
  pi5-led-blink-alpine
```

You can replace `PWR` with other available LEDs (e.g., `ACT`) by adjusting the mount path.

## Notes

-   The container must run on a Raspberry Pi 5 with Docker installed.
-   Using `--privileged` grants full device access; use limited mounts for safer experiments.
-   Check available LEDs on your device with:

    ```bash
    ls /sys/class/leds/
    ```
