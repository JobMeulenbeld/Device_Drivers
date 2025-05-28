#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>


#define DEVICE_NAME "char_device"
#define BUFFER_SIZE 20

static dev_t dev_number;
static struct cdev my_cdev;
static int head = 0;
static int tail = 0;
static char kernel_buffer[BUFFER_SIZE];
static int open_count = 0;

static struct class *char_class = NULL;
static struct device *char_device = NULL;

static int device_open(struct inode *inode, struct file *file)
{
    open_count++;
    pr_info("%s: Device opened %d time(s)\n", DEVICE_NAME, open_count);
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    pr_info("%s: Device closed\n", DEVICE_NAME);
    return 0;
}

static ssize_t device_read(struct file *file, char __user *user_buffer, size_t length, loff_t *offset)
{
    ssize_t bytes_read = 0;

    for(int i = 0; i < length; i++){
        if(tail == head) break;
        if(copy_to_user(user_buffer+i, &kernel_buffer[tail], 1) != 0) break;
        tail = ((tail + 1) % BUFFER_SIZE);
        bytes_read++;
    }

    pr_info("%s: Read %zd bytes\n", DEVICE_NAME, bytes_read);
    return bytes_read;
}

static ssize_t device_write(struct file *file, const char __user *user_buffer, size_t length, loff_t *offset)
{   
    ssize_t bytes_written = 0;

    for(int i = 0; i < length; i++){
        if((head+1) % BUFFER_SIZE == tail) break;
        if(copy_from_user(&kernel_buffer[head], user_buffer + i, 1) != 0) break;
        head = ((head + 1) % BUFFER_SIZE);
        bytes_written++;
    }

    pr_info("%s: Received %zd bytes\n", DEVICE_NAME, bytes_written);
    return bytes_written;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_release,
    .read = device_read,
    .write = device_write
};

static int __init char_device_init(void)
{
    int result;

    result = alloc_chrdev_region(&dev_number, 0, 1, DEVICE_NAME);
    if (result < 0) {
        pr_err("Failed to allocate char device region\n");
        return result;
    }

    cdev_init(&my_cdev, &fops);
    result = cdev_add(&my_cdev, dev_number, 1);
    if (result < 0) {
        pr_err("Failed to add cdev\n");
        unregister_chrdev_region(dev_number, 1);
        return result;
    }

    // Create device class
    char_class = class_create(DEVICE_NAME);

    if (IS_ERR(char_class)) {
        pr_err("Failed to create class\n");
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(char_class);
    }

    // Create device node automatically in /dev
    char_device = device_create(char_class, NULL, dev_number, NULL, DEVICE_NAME);
    if (IS_ERR(char_device)) {
        pr_err("Failed to create device\n");
        class_destroy(char_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev_number, 1);
        return PTR_ERR(char_device);
    }

    pr_info("%s: Module loaded, device major: %d minor: %d\n", DEVICE_NAME, MAJOR(dev_number), MINOR(dev_number));
    return 0;
}

static void __exit char_device_exit(void)
{
    device_destroy(char_class, dev_number);
    class_destroy(char_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev_number, 1);
    pr_info("%s: Module unloaded\n", DEVICE_NAME);
}


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Job");
MODULE_DESCRIPTION("Basic char device with read/write");

module_init(char_device_init);
module_exit(char_device_exit);