#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Job");
MODULE_DESCRIPTION("A simple Hello World Linux char driver");

static int __init hello_init(void)
{
    pr_info("HelloChar: Module loaded!\n");
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("HelloChar: Module unloaded.\n");
}

module_init(hello_init);
module_exit(hello_exit);
