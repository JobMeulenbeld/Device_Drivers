# Linux Device Drivers Collection

This repository folder contains a series of Linux kernel drivers. Each subdirectory contains a self-contained driver module, including source code, Makefiles, and supporting documentation.

---

## Drivers Overview

### ✅ `char_driver/`
A simple character device driver that demonstrates:
- Basic open/read/write/release operations
- Logging from kernel space using `printk`
- Manual creation of `/dev/char_device` using `mknod`

---

## Planned Drivers

These drivers will be added to the collection:

### `i2c_sensor_driver/`
A driver that interfaces with an I2C-based sensor (e.g., temperature or accelerometer), showing:
- Bus registration via `i2c_add_driver`
- Real hardware communication

### `gpio_irq_driver/`
A driver that handles GPIO input using interrupts, demonstrating:
- IRQ registration
- Asynchronous event handling
- Kernel <-> user notification patterns

### `sysfs_driver/`
Exposes internal kernel state using `sysfs`, allowing user-space to:
- Read device state
- Trigger kernel events through attribute files

### `ioctl_driver/`
Showcases advanced control interfaces via `ioctl()` calls for user-driven configuration and commands.

---

## Goal

The goal of this folder is to collect a number of drivers which have helped me develop an understanding of linux device drivers.

---

**Author:** Job Meulenbeld  
