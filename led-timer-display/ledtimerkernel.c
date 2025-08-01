#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/kobject.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/wait.h> // For wait queues
#include "myheader.h"

static unsigned int gpioLED1 = 60; // LSB
static unsigned int gpioLED2 = 49; // MSB2
static unsigned int gpioLED3 = 48; // MSB1
static unsigned int gpioLED4 = 47; // MSB

static unsigned int receivedvalue = 0;
static struct task_struct *counter_task = NULL;
static int counter_direction = 0; // 0: stopped, 1: up, -1: down
static DECLARE_WAIT_QUEUE_HEAD(wq);

static int display_binary_on_leds(unsigned int num)
{
    gpio_set_value(gpioLED1, (num & (1 << 0)) ? 1 : 0);
    gpio_set_value(gpioLED2, (num & (1 << 1)) ? 1 : 0);
    gpio_set_value(gpioLED3, (num & (1 << 2)) ? 1 : 0);
    gpio_set_value(gpioLED4, (num & (1 << 3)) ? 1 : 0);
    pr_info("LEDs set to display binary of %u\n", num);
    return 0;
}

static int counter_thread(void *data)
{
    int current_count = 0;

    while (!kthread_should_stop()) 
    {
        if (counter_direction == 1) { // Count up
            display_binary_on_leds(current_count);
            current_count++;
            if (current_count > 15) 
            {
                current_count = 0;
            }
        } else if (counter_direction == -1) { // Count down
            display_binary_on_leds(current_count);
            current_count--;
            if (current_count < 0) 
            {
                current_count = 15;
            }
        } 
        else 
        { wait_event_interruptible(wq, kthread_should_stop() || counter_direction != 0);
            if (kthread_should_stop())
                break;
        }
        msleep(500); 
    }
    pr_info("Counter thread exiting operation\n");
    return 0;
}

static long dh_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    unsigned int temp;
    int ret = 0;

    switch (cmd) {
    case SEND_INT:
        if (copy_from_user(&temp, (unsigned int __user *)arg, sizeof(temp))) {
            return -1;
        }
        if (temp > 15 || temp < 0) 
        {
            pr_warn("Received number %u is not in range setting it to 15 now\n", temp);
            temp = 15;
        }
        receivedvalue = temp;
        display_binary_on_leds(receivedvalue);
        pr_info("IOCTL has Received number %u \n", receivedvalue);
        break;
    case START_TIMER_UP:
        pr_info("Starting timer: Count Up\n");
        counter_direction = 1;
        wake_up_interruptible(&wq);
        break;
    case START_TIMER_DOWN:
        pr_info("Starting timer: Count Down\n");
        counter_direction = -1;
        wake_up_interruptible(&wq);
        break;
    case STOP_TIMER:
        pr_info("Stopping timer\n");
        counter_direction = 0;
        break;
    default:
        pr_warn("Unknown IOCTL value: 0x%x\n", cmd);
        ret = -1;
        break;
    }
    return ret;
}

static struct file_operations dh_fops = 
{
    .owner = THIS_MODULE,
    .unlocked_ioctl = dh_ioctl,
};

static struct class *dh_class;
static struct device *dh_device;
static int major_number;

static int __init my_init(void)
{
    int value = 0;

    pr_info("Init'ing GPIO LEDs...\n");

    major_number = register_chrdev(0, "DHdevice", &dh_fops);
    if (major_number < 0) 
    {
        pr_err("Failed to register char device\n");
        return major_number;
    }
    pr_info("Registered char device with major number %d\n", major_number);

    dh_class = class_create(THIS_MODULE, "DHclass");
    if (IS_ERR(dh_class)) 
    {
        pr_err("Failed to create class\n");
        unregister_chrdev(major_number, "DHdevice");
        return PTR_ERR(dh_class);
    }

    dh_device = device_create(dh_class, NULL, MKDEV(major_number, 0), NULL, "DHdevice");
    if (IS_ERR(dh_device)) 
    {
        pr_err("Failed to create device\n");
        class_destroy(dh_class);
        unregister_chrdev(major_number, "DHdevice");
        return PTR_ERR(dh_device);
    }

    value = gpio_request(gpioLED1, "sysfs_led1");
    if (value < 0) 
    {
        pr_err("Error requesting gpioLED1!\n");
        goto r_gpio_req1;
    }
    value = gpio_request(gpioLED2, "sysfs_led2");
    if (value < 0) 
    {
        pr_err("Error requesting gpioLED2!\n");
        goto r_gpio_req2;
    }
    value = gpio_request(gpioLED3, "sysfs_led3");
    if (value < 0) 
    {
        pr_err("Error requesting gpioLED3!\n");
        goto r_gpio_req3;
    }
    value = gpio_request(gpioLED4, "sysfs_led4");
    if (value < 0) 
    {
        pr_err("Error requesting gpioLED4!\n");
        goto r_gpio_req4;
    }

    gpio_direction_output(gpioLED1, 0);
    gpio_export(gpioLED1, false);

    gpio_direction_output(gpioLED2, 0);
    gpio_export(gpioLED2, false);

    gpio_direction_output(gpioLED3, 0);
    gpio_export(gpioLED3, false);

    gpio_direction_output(gpioLED4, 0);
    gpio_export(gpioLED4, false);

    display_binary_on_leds(0);

    counter_task = kthread_run(counter_thread, NULL, "LED_Counter_Thread");
    if (IS_ERR(counter_task)) 
    {
        pr_err("Failed to create counter thread\n");
        value = PTR_ERR(counter_task);
        goto r_task_fail;
    }

    return 0;

r_task_fail:
    gpio_set_value(gpioLED4, 0);
    gpio_unexport(gpioLED4);
    gpio_free(gpioLED4);
r_gpio_req4:
    gpio_set_value(gpioLED3, 0);
    gpio_unexport(gpioLED3);
    gpio_free(gpioLED3);
r_gpio_req3:
    gpio_set_value(gpioLED2, 0);
    gpio_unexport(gpioLED2);
    gpio_free(gpioLED2);
r_gpio_req2:
    gpio_set_value(gpioLED1, 0);
    gpio_unexport(gpioLED1);
    gpio_free(gpioLED1);
r_gpio_req1:
    device_destroy(dh_class, MKDEV(major_number, 0));
    class_destroy(dh_class);
    unregister_chrdev(major_number, "DHdevice");
    return -1;
}

static void __exit my_exit(void)
{
    if (counter_task) {
        kthread_stop(counter_task);
    }

    gpio_set_value(gpioLED1, 0);
    gpio_unexport(gpioLED1);
    gpio_free(gpioLED1);
    pr_info("GPIO %d freed up\n", gpioLED1);

    gpio_set_value(gpioLED2, 0);
    gpio_unexport(gpioLED2);
    gpio_free(gpioLED2);
    pr_info("GPIO %d freed up\n", gpioLED2);

    gpio_set_value(gpioLED3, 0);
    gpio_unexport(gpioLED3);
    gpio_free(gpioLED3);
    pr_info("GPIO %d freed up\n", gpioLED3);

    gpio_set_value(gpioLED4, 0);
    gpio_unexport(gpioLED4);
    gpio_free(gpioLED4);
    pr_info("GPIO %d freed up\n", gpioLED4);

    device_destroy(dh_class, MKDEV(major_number, 0));
    class_destroy(dh_class);
    unregister_chrdev(major_number, "DHdevice");

    printk(KERN_INFO "Good bye from %s!\n", KBUILD_MODNAME);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("A simple LED driver using GPIOs for binary display via IOCTL with counting");
