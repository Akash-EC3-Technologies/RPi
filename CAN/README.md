# CAN Frame Transmitter for Raspberry Pi with MCP2515

This experiment demonstrates how to transmit random CAN frames using an MCP2515 CAN controller on Raspberry Pi running inside a Docker container.

## Prerequisites

-   Raspberry Pi5
-   Docker installed on Raspberry Pi
-   MCP2515 CAN module (8MHz oscillator)
-   Jumper wires

## Hardware Connections

### MCP2515 to Raspberry Pi Connection

| MCP2515 Pin      | Raspberry Pi GPIO Pin  | Physical Pin Number |
| ---------------- | ---------------------- | ------------------- |
| VCC              | 3.3V Power             | Pin 1               |
| GND              | Ground                 | Pin 6               |
| CS (Chip Select) | GPIO8 (SPI CE0)        | Pin 24              |
| SO (MISO)        | GPIO9 (MISO)           | Pin 21              |
| SI (MOSI)        | GPIO10 (MOSI)          | Pin 19              |
| SCK (Clock)      | GPIO11 (SCLK)          | Pin 23              |
| CANH             | (To other CAN devices) | -                   |
| CANL             | (To other CAN devices) | -                   |

**Termination Resistor Connection:**

```
CANH ───╮
        │ 120Ω
CANL ───╯
```

**Pinout Reference**:

```text
Raspberry Pi 5 GPIO Header (J8)
┌─────────┬──────────┐
│ 1 3.3V  │ 2 5V     │
│ 3 GPIO2 │ 4 5V     │
│ 5 GPIO3 │ 6 GND    │
│ ...     │ ...      │
│ 19 MOSI │ 20 GND   │
│ 21 MISO │ 22 GPIO25|
│ 23 SCLK │ 24 CE0   │
└─────────┴──────────┘
```

## Setup Instructions

### 1. Configure System

```bash
# Install dependencies
sudo apt update
sudo apt install -y can-utils
```

### 2. Enable SPI Interface

```bash
sudo raspi-config # Navigate to Interface Options → SPI → Enable
```

### 3. Load device tree overlay for the MCP2515 CAN

-   open /boot/firmware/config.txt

```bash
sudo vim /boot/firmware/config.txt
```

-   Add this to the bottom of the config.txt:

```text
dtoverlay=mcp2515-can0,oscillator=16000000,interrupt=25,spimaxfrequency=10000000
```

-   Reboot

```
sudo reboot
```

### 4. Configure CAN Interface in Loopback Mode

```bash
# Bring down interface if up
sudo ip link set can0 down

# Set up loopback mode
sudo ip link set can0 up type can bitrate 500000 loopback on

# Verify interface
ip -d link show can0
# Should show "state ERROR-ACTIVE (loopback)"
```

## Building the Image

```bash
docker build -t can-transmitter .
```

## Running the Transmitter

```bash
docker run --network host \
           --cap-add=NET_ADMIN \
           --cap-add=NET_RAW \
           -it can-transmitter
```

The container will start transmitting random CAN frames in loopback mode.

### Monitoring CAN Traffic

In a separate terminal:

```bash
candump can0
```

You should see output like:

```
can0  123   [4]  01 02 03 04
can0  456   [2]  AA BB
```
