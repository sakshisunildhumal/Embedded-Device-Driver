#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>

static char *class_name =  NULL;
static char *device_name = NULL;
static int num_devs = 0;

module_param(class_name, charp, 0000);
MODULE_PARM_DESC(class_name, "Class name for the device (mandatory)");

module_param(device_name, charp, 0000);
MODULE_PARM_DESC(device_name, "Base name for the device (mandatory)");

module_param(num_devs, int, 0000);
MODULE_PARM_DESC(num_devs, "Number of devices to create (mandatory)");

dev_t dev = 0;
static struct class *dev_class = NULL;
static struct device **cdevices = NULL; 
static struct cdev *my_cdevs = NULL; 

static int __init my_mod_init(void);
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
    .release = my_release,
};

static int __init my_mod_init(void)
{
    int ans, i;

    pr_info("Module initialization started\n");

   
    if (!class_name) {
        pr_err("Parameter class_name is mandatory\n");
        return -EINVAL;
    }
    if (!device_name) {
        pr_err("Parameter device_name is mandatory\n");
        return -EINVAL;
    }
    if (num_devs <= 0) {
        pr_err("Parameter num_devs must be greater than 0\n");
        return -EINVAL;
    }

    pr_info("Module parameters: num_devs=%d, class_name=%s, device_name=%s\n",
            num_devs, class_name, device_name);

    ans = alloc_chrdev_region(&dev, 0, num_devs, device_name);
    if (ans < 0) {
        pr_err("Failed to allocate major/minor numbers\n");
        return ans;
    }
    pr_info("Allocated major:minor = %d:%d\n", MAJOR(dev), MINOR(dev));

    my_cdevs = kmalloc_array(num_devs, sizeof(struct cdev), GFP_KERNEL);
    cdevices = kmalloc_array(num_devs, sizeof(struct device *), GFP_KERNEL);
    if (!my_cdevs || !cdevices) {
        pr_err("Failed to allocate memory for cdev or devices\n");
        ans = -ENOMEM;
        goto unregister_chrdev;
    }

    dev_class = class_create(THIS_MODULE, class_name);
    if (IS_ERR(dev_class)) {
        pr_err("Failed to create device class %s\n", class_name);
        ans = PTR_ERR(dev_class);
        dev_class = NULL;
        goto free_mem;
    }


    for (i = 0; i < num_devs; i++) {
        cdev_init(&my_cdevs[i], &fops);
        ans = cdev_add(&my_cdevs[i], MKDEV(MAJOR(dev), MINOR(dev) + i), 1);
        if (ans < 0) {
            pr_err("Failed to add cdev %d\n", i);
            goto cleanup_devices;
        }

        cdevices[i] = device_create(dev_class, NULL, MKDEV(MAJOR(dev), MINOR(dev) + i), NULL, "%s%02d", device_name, i + 1);
        if (IS_ERR(cdevices[i])) {
            pr_err("Failed to create device %s%02d\n", device_name, i + 1);
            ans = PTR_ERR(cdevices[i]);
            cdev_del(&my_cdevs[i]);
            goto cleanup_devices;
        }
    }

    pr_info("Devices created successfully\n");
    return 0;

cleanup_devices:
    while (--i >= 0) {
        device_destroy(dev_class, MKDEV(MAJOR(dev), MINOR(dev) + i));
        cdev_del(&my_cdevs[i]);
    }
    class_destroy(dev_class);
free_mem:
    kfree(my_cdevs);
    kfree(cdevices);
unregister_chrdev:
    unregister_chrdev_region(dev, num_devs);
    return ans;
}

static void __exit my_mod_exit(void)
{
    int i;

    pr_info("Module exit started\n");

    if (dev_class) {
        for (i = 0; i < num_devs; i++) {
            device_destroy(dev_class, MKDEV(MAJOR(dev), MINOR(dev) + i));
        }
        class_destroy(dev_class);
    }

    if (my_cdevs) {
        for (i = 0; i < num_devs; i++) {
            cdev_del(&my_cdevs[i]);
        }
    }

    kfree(my_cdevs);
    kfree(cdevices);
    unregister_chrdev_region(dev, num_devs);

    pr_info("Module unloaded, major:minor freed\n");
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Character device module with parameters required from user at load");
