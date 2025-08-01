/* IOCTL on char device */

#define pr_fmt(fmt)     KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/ioctl.h>
#include <linux/slab.h>    
#include <linux/uaccess.h> 
#include <linux/timer.h>   
#include <linux/jiffies.h> 

#include "myheader.h" 

#define MY_CLASS_NAME   "cdac_cls"
#define MY_DEV_NAME     "cdac_dev"

// Global variables for character device
dev_t dev = 0;
static struct class *dev_class;
static struct device *cdevice;
static struct cdev my_cdev;


static struct timer_list my_kernel_timer;


static void my_timer_callback(struct timer_list *t)
{
    pr_info("Kernel timer expired! Rescheduling for 1 second.\n");


    mod_timer(&my_kernel_timer, jiffies + HZ);
}

static int __init my_mod_init(void);
static int my_open(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off);
static long my_ioctl(struct file *file, unsigned int cmd, unsigned long args);
static int my_release(struct inode *inode, struct file *file);
static void __exit my_mod_exit(void);


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
        case START_TIMER:
            pr_info("IOCTL: START_TIMER received.\n");
            
            if (timer_pending(&my_kernel_timer)) 
            {
                pr_info("Timer is already running. Not starting a new one.\n");
            } else 
            {
                mod_timer(&my_kernel_timer, jiffies + HZ);
                pr_info("Kernel timer started for 1 second periodicity.\n");
            }
            break;

        case STOP_TIMER:
            pr_info("IOCTL: STOP_TIMER received.\n");
           
            if (del_timer(&my_kernel_timer)) 
            {
                pr_info("Kernel timer stopped.\n");
            } else 
            {
                pr_info("Kernel timer was not active or already stopped.\n");
            }
            break;

        
        case MY_HW_READ:
            pr_info("IOCTL: MY_HW_READ received (placeholder).\n");
            
            {
                unsigned int dummy_value = 100; 
                if (copy_to_user((unsigned int *)arg, &dummy_value, sizeof(dummy_value))) 
                {
                    pr_err("Error copying dummy value to user space\n");
                    return -1; 
                }
                pr_info("Sent dummy value %d to user space.\n", dummy_value);
            }
            break;
        case MY_HW_WRITE:
            pr_info("IOCTL: MY_HW_WRITE received (placeholder).\n");
            
            {
                unsigned int dummy_value;
                if (copy_from_user(&dummy_value, (unsigned int *)arg, sizeof(dummy_value))) 
                {
                    pr_err("Error copying dummy value from user space\n");
                    return -1;
                }
                pr_info("Received dummy value %d from user space.\n", dummy_value);
            }
            break;

        default:
            pr_err("Undefined IOCTL command 0x%x!\n", cmd);
            return -1; 
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

static int __init my_mod_init(void)
{
    int ans;

    pr_info("Hello world from %s!\n", KBUILD_MODNAME);

    timer_setup(&my_kernel_timer, my_timer_callback, 0);

    ans = alloc_chrdev_region(&dev, 0, 1, MY_DEV_NAME);
    if (ans < 0)
    {
        pr_err("Error in major:minor allotment!\n");
        return -1;
    }
    pr_info("major:minor %d:%d allotted!\n", MAJOR(dev), MINOR(dev));


    cdev_init(&my_cdev, &fops);
    
    ans = cdev_add(&my_cdev, dev, 1);
    if (ans < 0)
    {
        pr_err("Could not add cdev to the kernel!\n");
        goto r_cdev; 
    }

    dev_class = class_create(THIS_MODULE, MY_CLASS_NAME);
    if (IS_ERR(dev_class))
    {
        pr_err("Could not create device class %s\n", MY_CLASS_NAME);
        ans = PTR_ERR(dev_class); 
        goto r_class; 
    }

  
    cdevice = device_create(dev_class, NULL, dev, NULL, MY_DEV_NAME);
    if (IS_ERR(cdevice))
    {
        pr_err("Could not create device %s\n", MY_DEV_NAME);
        ans = PTR_ERR(cdevice); 
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

    return ans; 
}

static void __exit my_mod_exit(void)
{
    pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

   
    if (timer_pending(&my_kernel_timer)) 
    {
        del_timer(&my_kernel_timer);
        pr_info("Kernel timer deleted during module exit.\n");
    }
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
MODULE_DESCRIPTION("IOCTLs for kernel timer control!");

