#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include "char_device_ioctl.h"

#define DEVICE_PATH "/dev/char_device"

int main() {
    int fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return 1;
    }

    printf("Device opened.\n");

    // Enable debug logging
    int debug = 1;
    if (ioctl(fd, CMD_SET_DEBUG, &debug) == -1)
        perror("ioctl CMD_SET_DEBUG failed");

    // Write something
    const char *msg = "Hello Kernel!";
    if (write(fd, msg, strlen(msg)) != (ssize_t)strlen(msg))
        perror("write failed");
    else
        printf("Wrote to device: %s\n", msg);

    // Get buffer fill level
    int fill = 0;
    if (ioctl(fd, CMD_GET_FILL, &fill) == -1)
        perror("ioctl CMD_GET_FILL failed");
    else
        printf("Buffer fill level: %d\n", fill);

    // Read back
    char read_buf[64] = {0};
    ssize_t bytes_read = read(fd, read_buf, sizeof(read_buf) - 1);
    if (bytes_read < 0)
        perror("read failed");
    else {
        read_buf[bytes_read] = '\0';
        printf("Read %zd bytes: \"%s\"\n", bytes_read, read_buf);
    }

    // Fill the buffer again, but don't read
    if (write(fd, msg, strlen(msg)) != (ssize_t)strlen(msg))
        perror("write failed");
    else
        printf("Wrote to device: %s\n", msg);

    // Clear the buffer
    if (ioctl(fd, CMD_CLEAR_BUFFER) == -1)
        perror("ioctl CMD_CLEAR_BUFFER failed");
    else
        printf("Buffer cleared.\n");

    // Confirm buffer is empty
    if (ioctl(fd, CMD_GET_FILL, &fill) == -1)
        perror("ioctl CMD_GET_FILL failed");
    else
        printf("Buffer fill level after clear: %d\n", fill);

    close(fd);
    return 0;
}
