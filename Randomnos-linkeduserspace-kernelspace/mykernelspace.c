#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/random.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>
#include "myheader.h" 

static int devMajor;
static char devName[] = "randomdev";

static int randomCount = 10;
static int randomUpper = 100;
static int randomLower = -100;

static int randomdevOpen(struct inode *inode, struct file *file)
{
    pr_info("Device opened\n");
    return 0;
}

static int randomdevRelease(struct inode *inode, struct file *file)
{
    pr_info("Device closed\n");
    return 0;
}

static ssize_t randomdevRead(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    int i;
    int bytesSent = 0;
    unsigned int rawval;
    int num;
    int numToRead = count / sizeof(int);

    if (*pos > 0)
        return 0;

    pr_info("Generating %d random numbers for user space\n", randomCount);

    if (numToRead > randomCount)
        numToRead = randomCount;

    for (i = 0; i < numToRead; i++)
    {
        get_random_bytes(&rawval, sizeof(rawval));
        num = (int)((rawval % (randomUpper - randomLower + 1)) + randomLower);
        
        if (copy_to_user(buf + bytesSent, &num, sizeof(int)))
        {
            pr_err("Failed to copy random number to user space\n");
            return -1;
        }
        bytesSent += sizeof(int);
    }
    *pos += bytesSent;
    return bytesSent;
}

static long randomdevIoctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int val;

    switch (cmd)
    {
        case SET_COUNT:
            if (copy_from_user(&val, (int __user *)arg, sizeof(int)))
                return -1;
            if (val <= 0)
            {
                pr_err("Invalid count\n");
                return -1;
            }
            randomCount = val;
            pr_info("Random count set to %d\n", randomCount);
            break;
        case SET_UPPER:
            if (copy_from_user(&val, (int __user *)arg, sizeof(int)))
                return -1;
            
            if (val <= randomLower || (val == -randomLower && val != 0))
            {
                pr_err("Invalid upper limit\n");
                return -1;
            }
            randomUpper = val;
            pr_info("Random upper limit set to %d\n", randomUpper);
            break;
        case SET_LOWER:
            if (copy_from_user(&val, (int __user *)arg, sizeof(int)))
                return -1;
            
            if (val >= randomUpper || (val == -randomUpper && val != 0))
            {
                pr_err("Invalid lower limit\n");
                return -1;
            }
            randomLower = val;
            pr_info("Random lower limit set to %d\n", randomLower);
            break;
        default:
            pr_warn("Unknown ioctl command\n");
            return -1;
    }
    return 0;
}

static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = randomdevOpen,
    .release = randomdevRelease,
    .read = randomdevRead,
    .unlocked_ioctl = randomdevIoctl,
};

static int __init myInit(void)
{
    pr_info("Random device module init\n");
    devMajor = register_chrdev(0, devName, &fops);
    if (devMajor < 0)
    {
        pr_err("Failed to register char device\n");
        return devMajor;
    }
    pr_info("Registered char device with major %d\n", devMajor);
    pr_info("Create device node with: mknod /dev/%s c %d 0\n", devName, devMajor);
    return 0;
}

static void __exit myExit(void)
{
    pr_info("Random device module exit\n");
    unregister_chrdev(devMajor, devName);
}

module_init(myInit);
module_exit(myExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("User-kernel random number generator");
