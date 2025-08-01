#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include "myheader.h" 

dev_t dev = 0;
struct class *dev_class;
struct device *cdevice;
struct cdev my_cdev;
char *kbuf;

struct file_operations fops = 
{
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .release = my_release,
};

static int __init my_mod_init(void)
{
    int ans;

    kbuf = (char *)kmalloc(MY_KBUF_SZ, GFP_KERNEL);
    if (!kbuf)
    {
        pr_err("Cannot allocate kernel memory!\n");
        return -1;
    }

    pr_info("Hello world from mod35!\n");
    ans = alloc_chrdev_region(&dev, 0, 1, MY_DEV_NAME);
    if (ans<0)
    {
        pr_err("Error in major:minor allotment!\n");
        goto r_kbuf;
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
    pr_info("From splitted 3 code project\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    cdev_del(&my_cdev);
r_cdev:
    unregister_chrdev_region(dev, 1);
r_kbuf:
    kfree(kbuf);

    return -1;
}

static void __exit my_mod_exit(void)
{
    pr_info("Goodbye world from mod351!\n");
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);
    kfree(kbuf);
    pr_info("major:minor numbers freed up...\n");
    return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Passing data between user and kernel spaces module!");
