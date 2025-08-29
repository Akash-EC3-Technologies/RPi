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
| VCC              | 5V Power               | Pin 4               |
| GND              | Ground                 | Pin 6               |
| CS (Chip Select) | GPIO8 (SPI CE0)        | Pin 24              |
| SO (MISO)        | GPIO9 (MISO)           | Pin 21              |
| SI (MOSI)        | GPIO10 (MOSI)          | Pin 19              |
| SCK (Clock)      | GPIO11 (SCLK)          | Pin 23              |
| INT              | GPIO25                 | Pin 22              |
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

<img width="755" height="290" alt="image" src="https://github.com/user-attachments/assets/a4e79c99-5acb-4d67-821c-59051e3cc6b4" />

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
dtoverlay=mcp2515-can0,oscillator=8000000,interrupt=25,spimaxfrequency=1000000
```

-   Reboot

```
sudo reboot
```

### 4. Configure CAN Interface

```bash
# Bring down interface if up
sudo ip link set can0 down

sudo ip link set can0 up type can bitrate 125000 on

# Verify interface
ip -d link show can0
# Should show "state ERROR-ACTIVE"
```

# Building and Running TailLampApplication

```bash
cd TailLamp
make build
make run
```

# Building and Running BreakPedalApplication

```bash
cd BrealPedal
make build
make run
```

## Building the Docker Image

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
