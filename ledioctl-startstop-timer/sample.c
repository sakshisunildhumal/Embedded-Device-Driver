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

#define GPIO_LED_0 47
#define GPIO_LED_1 48
#define GPIO_LED_2 49
#define GPIO_LED_3 60

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
    gpio_set_value(GPIO_LED_0, (val >> 3) & 0x1);
    gpio_set_value(GPIO_LED_1, (val >> 2) & 0x1);
    gpio_set_value(GPIO_LED_2, (val >> 1) & 0x1);
    gpio_set_value(GPIO_LED_3, val & 0x1);
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
        timstate = TIMER_UP;
        count = 0;
        ledstate(count);
        hrtimer_start(&my_hrtimer, ktime_set(1, 0), HRTIMER_MODE_REL);
        break;
    case START_TIMER_DOWN:
        timstate = TIMER_DOWN;
        count = 0xF;
        ledstate(count);
        hrtimer_start(&my_hrtimer, ktime_set(1, 0), HRTIMER_MODE_REL);
        break;
    case STOP_TIMER:
        timstate = TIMER_STOP;
        hrtimer_cancel(&my_hrtimer);
        ledstate(0);
        break;
    default:
        mutex_unlock(&timer_mutex);
        return -EINVAL;
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

    gpio_request(GPIO_LED_0, "LED0");
    gpio_direction_output(GPIO_LED_0, 0);
    gpio_request(GPIO_LED_1, "LED1");
    gpio_direction_output(GPIO_LED_1, 0);
    gpio_request(GPIO_LED_2, "LED2");
    gpio_direction_output(GPIO_LED_2, 0);
    gpio_request(GPIO_LED_3, "LED3");
    gpio_direction_output(GPIO_LED_3, 0);

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

    gpio_free(GPIO_LED_0);
    gpio_free(GPIO_LED_1);
    gpio_free(GPIO_LED_2);
    gpio_free(GPIO_LED_3);

    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);

    mutex_destroy(&timer_mutex);
}

module_init(gpio_timer_led_init);
module_exit(gpio_timer_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("LKM to control 4 LEDs with timer via ioctl");


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>

#include "myheader.h"

#define DEVICE_PATH "/dev/cdac_dev"

#define ioc 'k'
#define START_TIMER_UP _IO(ioc, 1)
#define START_TIMER_DOWN _IO(ioc, 2)
#define STOP_TIMER _IO(ioc, 3)

void usage(const char *prog)
{
    printf("Usage:\n");
    printf("  %s START_UP    - Start counting LEDs UP\n", prog);
    printf("  %s START_DOWN  - Start counting LEDs DOWN\n", prog);
    printf("  %s STOP        - Stop the timer and turn off LEDs\n", prog);
}

int main(int argc, char *argv[])
{
    int fd;
    unsigned int cmd;

    if (argc != 2) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (strcmp(argv[1], "start_up") == 0) {
        cmd = START_TIMER_UP;
    } else if (strcmp(argv[1], "start_down") == 0) {
        cmd = START_TIMER_DOWN;
    } else if (strcmp(argv[1], "stop") == 0) {
        cmd = STOP_TIMER;
    } else {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }

    if (ioctl(fd, cmd) < 0) {
        perror("Failed to send ioctl");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("IOCTL command '%s' sent successfully.\n", argv[1]);
    close(fd);
    return EXIT_SUCCESS;
}

alter this code such that do not use # define for led and do not change my variable names change the logic such that when user space code is run it will display a switch case from which we will enter 1 2 or 3 1 for START_TIMER_UP 2 for START_TIMER_DOWN and 3 for STOP_TIMER and do not any comments from your side use simplest possibe format 
