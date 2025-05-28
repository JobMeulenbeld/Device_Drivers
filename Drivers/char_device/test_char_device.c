#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <assert.h>
#include "char_device_ioctl.h"

#define DEVICE_PATH "/dev/char_device"

int main() {
    int fd = open(DEVICE_PATH, O_RDWR);
    assert(fd >= 0 && "Failed to open /dev/char_device");

    printf("Device opened.\n");

    // Enable debug logging
    int debug = 1;
    assert(ioctl(fd, CMD_SET_DEBUG, &debug) != -1 && "ioctl CMD_SET_DEBUG failed");

    // Write a message
    const char *msg = "Hello Kernel!";
    ssize_t written = write(fd, msg, strlen(msg));
    assert(written == (ssize_t)strlen(msg) && "write failed or incomplete");

    printf("Wrote to device: %s\n", msg);

    // Get buffer fill level
    int fill = 0;
    assert(ioctl(fd, CMD_GET_FILL, &fill) != -1 && "ioctl CMD_GET_FILL failed");
    assert(fill == (int)strlen(msg) && "Buffer fill level mismatch");

    printf("Buffer fill level: %d\n", fill);

    // Read back
    char read_buf[64] = {0};
    ssize_t bytes_read = read(fd, read_buf, sizeof(read_buf) - 1);
    assert(bytes_read == (ssize_t)strlen(msg) && "read failed or incomplete");

    read_buf[bytes_read] = '\0';
    assert(strncmp(read_buf, msg, bytes_read) == 0 && "Read content mismatch");

    printf("Read %zd bytes: \"%s\"\n", bytes_read, read_buf);

    // Fill the buffer again
    written = write(fd, msg, strlen(msg));
    assert(written == (ssize_t)strlen(msg) && "Second write failed or incomplete");

    printf("Wrote to device again: %s\n", msg);

    // Clear the buffer
    assert(ioctl(fd, CMD_CLEAR_BUFFER) != -1 && "ioctl CMD_CLEAR_BUFFER failed");
    printf("Buffer cleared.\n");

    // Confirm buffer is empty
    assert(ioctl(fd, CMD_GET_FILL, &fill) != -1 && "ioctl CMD_GET_FILL failed");
    assert(fill == 0 && "Buffer not empty after clear");

    printf("Buffer fill level after clear: %d\n", fill);

    close(fd);
    return 0;
}
