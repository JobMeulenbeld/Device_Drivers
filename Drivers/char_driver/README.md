# `char_driver` – Simple Linux Character Device Driver

This is a basic Linux kernel module that implements a simple **character device**. It demonstrates how to:

- Create and register a character device
- Handle `open()`, `read()`, `write()`, and `release()` system calls
- Exchange data between user space and kernel space
- Log device activity using `printk`
- Manually create a `/dev/` entry to interact with the device

---

## File Structure

```
char_driver/
├── char_driver.c     # Source code of the driver
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
sudo insmod char_driver.ko
sudo dmesg | tail -n 20
```

You’ll see a log like:

```
char_device: Module loaded, device major: 234 minor: 0
```

Take note of the **major number**.

---

## Creating the Device File

Replace `234` with the actual major number from `dmesg`:

```bash
sudo mknod /dev/char_device c 234 0
sudo chmod 666 /dev/char_device
```

Now you can interact with the device using standard commands thanks to the `chmod 666` command (gives read and write permissions) which is equivelant to: `chmod u=rw,g=rw,o=rw`.

---

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

This reads up to 1024 bytes from the kernel buffer.

---

## Unloading the Module

```bash
sudo rmmod char_driver
sudo dmesg | tail -n 20
```

---

## 🛠 Features

- Allocates a buffer of 1024 bytes in kernel space
- Logs all operations: open, read, write, and close
- Maintains an open-count tracker
- Simple interface for testing driver interactions

---

## Next Steps / TODO

- [ ] Add automatic `/dev/char_device` creation using `device_create()`
- [ ] Implement a circular buffer
- [ ] Add `ioctl()` interface for control commands
- [ ] Create a sysfs attribute to expose internal state

---

## Author

**Job Meulenbeld**  
> Embedded systems engineer | Kernel enthusiast |  
> `"Job is cool man"` — from `/dev/char_device`
