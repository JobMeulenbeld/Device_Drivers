# `char_device` – Simple Linux Character Device Driver

This is a basic Linux kernel module that implements a simple **character device**. It demonstrates how to:

- Create and register a character device
- Handle `open()`, `read()`, `write()`, and `release()` system calls
- Exchange data between user space and kernel space
- Log device activity using `printk`
- Automatically create a `/dev/` entry to interact with the device

---

## Requirements

- Secure boot must be disabled. Otherwise the kernel will not allow to instantiate unsigned nodes (/dev/yourdriver)

## File Structure

```
char_device/
├── char_device.c     # Source code of the driver
├── Makefile          # Build instructions
└── README.md         # This file
```

---

## Building the Module

Build the module:

```bash
make
```

---

## Loading the Module

```bash
sudo insmod char_device.ko
sudo dmesg | tail -n 20
```

You’ll see a log like:

```
char_device: Module loaded, device major: 234 minor: 0
```

Take note of the **major number**.

---

## Granting permission

The rule file `99-char_device.rule` sets the right permissions for the driver. Copy the file with:
```bash
sudo cp 99-char_device.rules /etc/udev/rules.d/
```

Reload and apply:

```bash
sudo udevadm control --reload-rules
sudo udevadm trigger
```

Reload the module:

```bash
sudo rmmod char_device
sudo insmod char_device.ko
```

## Writing to the Device

```bash
echo "Hello kernel" > /dev/char_device
```

This writes data to the kernel buffer and logs the message.

---

## Reading from the Device

```bash
cat /dev/char_device
```

This reads up to 20 bytes from the kernel buffer.

---

## Unloading the Module

```bash
sudo rmmod char_device
sudo dmesg | tail -n 20
```

---

## Features

- Allocates a circular buffer of 20 bytes in kernel space
- Logs all operations: open, read, write, and close when debug enabled through ioctl
- Added ioctl commands to control the char_device (see: [char_device_ioctl.h](./char_device_ioctl.h) & [test_char_device.c](./test_char_device.c))
- Maintains an open-count tracker
- Simple interface for testing driver interactions

---

## Next Steps / TODO

- [x] Add automatic `/dev/char_device` creation using `device_create()`
- [x] Implement a circular buffer
- [x] Add `ioctl()` interface for control commands
- [ ] Create a sysfs attribute to expose internal state

---

## Author

**Job Meulenbeld**  
> Embedded systems engineer | Kernel enthusiast |  
> `"Job is cool man"` — from `/dev/char_device`
