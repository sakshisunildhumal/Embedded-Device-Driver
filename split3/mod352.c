#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include "myheader.h" 

int my_open(struct inode *inode, struct file *file)
{
    pr_info("My open function %s called\n", __func__);
    return 0;
}


int my_release(struct inode *inode, struct file *file)
{
    pr_info("My release function %s called\n", __func__);
    return 0;
}


MODULE_LICENSE("GPL");
