/* USer-space - Kernel-space interactions using char device */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define MY_CLASS_NAME	"cdac_cls"
#define MY_DEV_NAME	"cdac_dev"
#define MY_KBUF_SZ	(1024)

dev_t dev = 0;
static struct class *dev_class;
static struct device *cdevice;
static struct cdev my_cdev;

// kernel memory buffer ptr
char *kbuf;

// function prototypes
static int __init my_mod_init(void);
static int my_open(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off);
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
	if ( copy_to_user(buf, kbuf + *off, len) )
	{
		pr_err("Error in data write to userspace!\n");
	}
	pr_info("Data written to userspace\n");
	*off += len;
	return len;
}

static ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off)
{
	pr_info("My write function %s called\n", __func__);
	if ( copy_from_user(kbuf, buf, len) )
	{
		pr_err("Error in data read from userspace!\n");
	}
	pr_info("Data read from userspace!\n");
	return len;
}

static int my_release(struct inode *inode, struct file *file)
{
	pr_info("My release function %s called\n", __func__);
	return 0;
}

// create a fops struct
static struct file_operations fops = {
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

	// initialize a cdev
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
r_kbuf:
	kfree(kbuf);

	return -1;
}

static void __exit my_mod_exit(void)
{
	pr_info("Goodbye world from mod35!\n");
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

