#define pr_fmt(fmt)    KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>

#include "myheader.h"

#define MY_CLASS_NAME    "DHclass"
#define MY_DEV_NAME    "DHdevice"

static unsigned int value;

dev_t dev = 0;
static struct class *dev_class;
static struct device *cdevice;
static struct cdev my_cdev;

static int __init my_mod_init(void);
static int my_open(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off);
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long args);
static int my_release(struct inode *inode, struct file *file);
static void __exit my_mod_exit(void);

static void reverse_string(char* str, int length);
static char *to_binary(unsigned int num);
static char *to_octal(unsigned int num);
static char *to_hexadecimal(unsigned int num);

static int my_open(struct inode *inode, struct file *file)
{
    pr_info("My open function %s called\n", __func__);
    return 0;
}

static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off)
{
    pr_info("My read function %s called\n", __func__);
    return 0;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
    pr_info("My write function %s called\n", __func__);
    return len;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
        case SEND_INT:
            if ( copy_from_user(&value, (unsigned int *)arg, sizeof(value)) )
            {
                pr_err("Error receiving value via IOCTL\n");
            }
            pr_info("Received unsigned int: %u\n", value);
            pr_info("Binary: %s\n", to_binary(value));
            pr_info("Octal: %s\n", to_octal(value));
            pr_info("Hexadecimal: %s\n", to_hexadecimal(value));
            break;
        default:
            pr_err("Undefined command!\n");
            break;
    }
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    pr_info("My release function %s called\n", __func__);
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .unlocked_ioctl = my_ioctl,
    .release = my_release,
};

static void reverse_string(char* str, int length) 
{
    int start = 0;
    int end = length - 1;
    char temp;
    while (start < end) 
    {
        temp = *(str + start);
        *(str + start) = *(str + end);
        *(str + end) = temp;
        start++;
        end--;
    }
}

static char *to_binary(unsigned int num) 
{
    static char binarystr[35]; 
    int i = 0;

    if (num == 0) 
    {
        binarystr[0] = '0';
        binarystr[1] = 'b';
        binarystr[2] = '0';
        binarystr[3] = '\0';
        return binarystr;
    }

    while (num > 0) 
    {
        binarystr[i++] = (num % 2) + '0';
        num /= 2;
    }

    binarystr[i++] = 'b';
    binarystr[i++] = '0';
    binarystr[i] = '\0';

    reverse_string(binarystr, i);
    return binarystr;
}

static char *to_octal(unsigned int num) 
{
    static char octalstr[14]; 
    int i = 0;

    if (num == 0) 
    {
        octalstr[0] = '0';
        octalstr[1] = 'o';
        octalstr[2] = '0';
        octalstr[3] = '\0';
        return octalstr;
    }

    while (num > 0) 
    {
        octalstr[i++] = (num % 8) + '0';
        num /= 8;
    }

    octalstr[i++] = 'o';
    octalstr[i++] = '0';
    octalstr[i] = '\0';

    reverse_string(octalstr, i);
    return octalstr;
}

static char *to_hexadecimal(unsigned int num) 
{
    static char hexstr[11]; 
    int i = 0;
    int rem;

    if (num == 0) 
    {
        hexstr[0] = '0';
        hexstr[1] = 'x';
        hexstr[2] = '0';
        hexstr[3] = '\0';
        return hexstr;
    }

    while (num > 0) 
    {
        rem = num % 16;
        if (rem < 10) 
        {
            hexstr[i++] = rem + '0';
        } else 
        {
            hexstr[i++] = rem + 'a' - 10;
        }
        num /= 16;
    }

    hexstr[i++] = 'x';
    hexstr[i++] = '0';
    hexstr[i] = '\0';

    reverse_string(hexstr, i);
    return hexstr;
}


static int __init my_mod_init(void)
{
    int ans;

    pr_info("Hello world from %s!\n", KBUILD_MODNAME);
    ans = alloc_chrdev_region(&dev, 0, 1, MY_DEV_NAME);
    if (ans<0)
    {
        pr_err("Error in major:minor allotment!\n");
        return -1;
    }
    pr_info("major:minor %d:%d allotted!\n", MAJOR(dev),MINOR(dev));

    cdev_init(&my_cdev, &fops);

    ans = cdev_add(&my_cdev, dev, 1);
    if (ans<0)
    {
        pr_err("Could not add cdev to the kernel!\n");
        goto r_cdev;
    }

    dev_class = class_create(THIS_MODULE, MY_CLASS_NAME);
    if (IS_ERR(dev_class))
    {
        pr_err("Could not create device class %s\n", MY_CLASS_NAME);
        goto r_class;
    }

    cdevice = device_create(dev_class, NULL, dev, NULL, MY_DEV_NAME);
    if (IS_ERR(cdevice))
    {
        pr_err("Could not create device %s\n", MY_DEV_NAME);
        goto r_device;
    }
    pr_info("Device %s under class %s created with success\n", 
            MY_DEV_NAME, MY_CLASS_NAME);
    return 0;

r_device:
        class_destroy(dev_class);
r_class:
        cdev_del(&my_cdev);
r_cdev:
        unregister_chrdev_region(dev, 1);

    return -1;
}

static void __exit my_mod_exit(void)
{
    pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("major:minor numbers freed up...\n");
    return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("IOCTL on char device!");
