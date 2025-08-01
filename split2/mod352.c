#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

// Declare kbuf as extern since it's defined and exported in my_char_device_main.c
extern char *kbuf;

int my_open(struct inode *inode, struct file *file)
{
    pr_info("My open function %s called\n", __func__);
    return 0;
}
EXPORT_SYMBOL(my_open); // Export this function

ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    pr_info("My read function %s called\n", __func__);
    if ( copy_to_user(buf, kbuf + *off, len) )
    {
        pr_err("Error in data write to userspace!\n");
    }
    pr_info("Data written to userspace\n");
    *off += len;
    return len;
}
EXPORT_SYMBOL(my_read); // Export this function

ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("My write function %s called\n", __func__);
    if ( copy_from_user(kbuf, buf, len) )
    {
        pr_err("Error in data read from userspace!\n");
    }
    pr_info("Data read from userspace!\n");
    return len;
}
EXPORT_SYMBOL(my_write); // Export this function

int my_release(struct inode *inode, struct file *file)
{
    pr_info("My release function %s called\n", __func__);
    return 0;
}
EXPORT_SYMBOL(my_release); // Export this function

// It's good practice to have MODULE_LICENSE in every .c file that contributes to a module,
// even if it's technically only needed once for the final module.
// This resolves the 'missing MODULE_LICENSE()' warning.
MODULE_LICENSE("GPL");
