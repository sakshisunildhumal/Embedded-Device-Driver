#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/mutex.h>

#include "myheader.h"

#define MY_CLASS_NAME "cdac_cls"
#define MY_DEV_NAME "cdac_dev"

static dev_t dev = 0;
static struct class *dev_class;
static struct device *cdevice;
static struct cdev my_cdev;

static struct hrtimer my_hrtimer;
static DEFINE_MUTEX(timer_mutex);

enum timval
{
    TIMER_STOP = 0,
    TIMER_UP,
    TIMER_DOWN
};

static enum timval timstate = TIMER_STOP;
static unsigned int count = 0;

static void ledstate(unsigned int val)
{
    gpio_set_value(47, (val >> 3) & 0x1);
    gpio_set_value(48, (val >> 2) & 0x1);
    gpio_set_value(49, (val >> 1) & 0x1);
    gpio_set_value(60, val & 0x1);
}

static enum hrtimer_restart timer_callback(struct hrtimer *hrtmr)
{
    mutex_lock(&timer_mutex);

    if (timstate == TIMER_UP)
        count = (count + 1) & 0xF;
    else if (timstate == TIMER_DOWN)
        count = (count - 1) & 0xF;
    else {
        ledstate(0);
        mutex_unlock(&timer_mutex);
        return HRTIMER_NORESTART;
    }

    ledstate(count);
    hrtimer_forward_now(hrtmr, ktime_set(1, 0));

    mutex_unlock(&timer_mutex);
    return HRTIMER_RESTART;
}

static int my_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    return 0;
}

static long my_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    mutex_lock(&timer_mutex);

    switch (cmd) {
    case START_TIMER_UP:
    	pr_info("In START_TIMER_UP function\n");
    	pr_info("count value is %d \n", count);
        timstate = TIMER_UP;
        //count = 0;
        ledstate(count);
        hrtimer_start(&my_hrtimer, ktime_set(1, 0), HRTIMER_MODE_REL);
        break;
    case START_TIMER_DOWN:
        pr_info("In START_TIMER_DOWN function\n");
        pr_info("count value is %d \n", count);
        timstate = TIMER_DOWN;
        //count = 0xF;
        ledstate(count);
        hrtimer_start(&my_hrtimer, ktime_set(1, 0), HRTIMER_MODE_REL);
        break;
    case STOP_TIMER:
    	count = 0x00;
    	pr_info("In STOP_TIMER function\n");
    	pr_info("count value is %d \n", count);
        timstate = TIMER_STOP;
        hrtimer_cancel(&my_hrtimer);   
        ledstate(0);
        break;
    default:
        mutex_unlock(&timer_mutex);
        return -1;
    }

    mutex_unlock(&timer_mutex);
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_release,
    .unlocked_ioctl = my_ioctl,
};

static int __init gpio_timer_led_init(void)
{
    int ret;

    ret = alloc_chrdev_region(&dev, 0, 1, MY_DEV_NAME);
    if (ret < 0) return ret;

    cdev_init(&my_cdev, &fops);
    ret = cdev_add(&my_cdev, dev, 1);
    if (ret < 0) {
        unregister_chrdev_region(dev, 1);
        return ret;
    }

    dev_class = class_create(THIS_MODULE, MY_CLASS_NAME);
    if (IS_ERR(dev_class)) {
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev, 1);
        return PTR_ERR(dev_class);
    }

    cdevice = device_create(dev_class, NULL, dev, NULL, MY_DEV_NAME);
    if (IS_ERR(cdevice)) {
        class_destroy(dev_class);
        cdev_del(&my_cdev);
        unregister_chrdev_region(dev, 1);
        return PTR_ERR(cdevice);
    }

    gpio_request(47, "LED0");
    gpio_direction_output(47, 0);
    gpio_request(48, "LED1");
    gpio_direction_output(48, 0);
    gpio_request(49, "LED2");
    gpio_direction_output(49, 0);
    gpio_request(60, "LED3");
    gpio_direction_output(60, 0);

    mutex_init(&timer_mutex);
    hrtimer_init(&my_hrtimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
    my_hrtimer.function = timer_callback;

    return 0;
}

static void __exit gpio_timer_led_exit(void)
{
    mutex_lock(&timer_mutex);
    timstate = TIMER_STOP;
    hrtimer_cancel(&my_hrtimer);
    ledstate(0);
    mutex_unlock(&timer_mutex);

    gpio_free(47);
    gpio_free(48);
    gpio_free(49);
    gpio_free(60);

    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);

    mutex_destroy(&timer_mutex);
    pr_info("Exiting from the led timer code %d \n");
}

module_init(gpio_timer_led_init);
module_exit(gpio_timer_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("LKM to control 4 LEDs with timer via ioctl");

