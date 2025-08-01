#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include "myheader.h" 

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


MODULE_LICENSE("GPL");
