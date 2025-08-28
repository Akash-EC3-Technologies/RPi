#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

// Helper: write string to sysfs file
static void writeFile(const char *path, const char *value)
{
    int fd = open(path, O_WRONLY);
    if (fd < 0)
    {
        perror("open");
        exit(1);
    }
    if (write(fd, value, strlen(value)) < 0)
    {
        perror("write");
        exit(1);
    }
    close(fd);
}

// Helper: read a sysfs file
static int readFile(const char *path, char *buf, size_t size)
{
    int fd = open(path, O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        exit(1);
    }
    int len = read(fd, buf, size - 1);
    if (len < 0)
    {
        perror("read");
        exit(1);
    }
    buf[len] = '\0';
    close(fd);
    return len;
}

void gpio_init(int pin)
{
    char path[64], pinStr[8];
    snprintf(pinStr, sizeof(pinStr), "%d", pin);

    // Export
    writeFile("/sys/class/gpio/export", pinStr);

    // Set direction to "out"
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/direction", pin);
    writeFile(path, "out");
}

void gpio_set(int pin, int value)
{
    char path[64];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    writeFile(path, value ? "1" : "0");
}

int gpio_read(int pin)
{
    char path[64], buf[4];
    snprintf(path, sizeof(path), "/sys/class/gpio/gpio%d/value", pin);
    readFile(path, buf, sizeof(buf));
    return (buf[0] == '1') ? 1 : 0;
}

void gpio_cleanup(int pin)
{
    char pinStr[8];
    snprintf(pinStr, sizeof(pinStr), "%d", pin);
    writeFile("/sys/class/gpio/unexport", pinStr);
}
