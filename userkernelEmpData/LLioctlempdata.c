#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/list.h>

#include "header.h"

#define DEVICE_NAME "cdac_dev"
#define CLASS_NAME  "cdac_class"

static dev_t major_minor_num;
static struct class *cdac_dev_class;
static struct cdev cdac_cdev;

struct cdac_employee_node {
    info data;
    struct list_head list;
};

static LIST_HEAD(employee_list);

static int cdac_dev_open(struct inode *inode, struct file *file) {
    pr_info("Device opened.\n");
    return 0;
}

static int cdac_dev_release(struct inode *inode, struct file *file) {
    pr_info("Device closed.\n");
    return 0;
}

static int is_empid_duplicate(int empid) {
    struct cdac_employee_node *current_node;
    list_for_each_entry(current_node, &employee_list, list) {
        if (current_node->data.empid == empid) {
            return 1;
        }
    }
    return 0;
}

static long cdac_dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    long retval = 0;
    info temp_employee_data;
    struct cdac_employee_node *new_node, *current_node;
    int list_size = 0;
    int i;

    switch (cmd) {
        case SEND_EMPLOYEE_DATA:
            if (copy_from_user(&temp_employee_data, (info __user *)arg, sizeof(info))) {
                pr_err("Failed to copy employee data from user space.\n");
                retval = -EFAULT;
                goto out;
            }

            if (is_empid_duplicate(temp_employee_data.empid)) {
                pr_warn("Duplicate Employee ID %d received. Not adding.\n", temp_employee_data.empid);
                retval = -EEXIST;
                goto out;
            }

            new_node = kmalloc(sizeof(struct cdac_employee_node), GFP_KERNEL);
            if (!new_node) {
                pr_err("Failed to allocate memory for new employee node.\n");
                retval = -ENOMEM;
                goto out;
            }

            for (i = 0; i < 20; i++) {
                new_node->data.empname[i] = temp_employee_data.empname[i];
                if (temp_employee_data.empname[i] == '\0') break;
            }
            new_node->data.gender = temp_employee_data.gender;
            new_node->data.empid = temp_employee_data.empid;
            
            list_add_tail(&new_node->list, &employee_list);

            pr_info("Added employee: Name=%s, ID=%d, Gender=%c\n",
                   new_node->data.empname, new_node->data.empid, new_node->data.gender);
            break;

        case GET_LIST_SIZE:
            list_for_each_entry(current_node, &employee_list, list) {
                list_size++;
            }
            if (copy_to_user((int __user *)arg, &list_size, sizeof(int))) {
                pr_err("Failed to copy list size to user space.\n");
                retval = -EFAULT;
                goto out;
            }
            pr_info("Returned list size: %d\n", list_size);
            break;

        case PRINT_LIST:
            pr_info("--- Contents of Employee List ---\n");
            if (list_empty(&employee_list)) {
                pr_info("The employee list is empty.\n");
            } else {
                list_for_each_entry(current_node, &employee_list, list) {
                    pr_info("  Name: %s, ID: %d, Gender: %c\n",
                           current_node->data.empname, current_node->data.empid, current_node->data.gender);
                }
            }
            pr_info("--- End of Employee List ---\n");
            break;

        default:
            pr_warn("Unknown IOCTL command 0x%x\n", cmd);
            retval = -ENOTTY;
            break;
    }

out:
    return retval;
}

static const struct file_operations cdac_fops = {
    .owner          = THIS_MODULE,
    .open           = cdac_dev_open,
    .release        = cdac_dev_release,
    .unlocked_ioctl = cdac_dev_ioctl,
};

static int __init cdac_dev_init(void) {
    int ret;

    pr_info("Initializing CDAC Employee Device Driver...\n");

    ret = alloc_chrdev_region(&major_minor_num, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        pr_err("Failed to allocate major/minor number. Error: %d\n", ret);
        return ret;
    }
    pr_info("Allocated device with Major: %d, Minor: %d\n",
           MAJOR(major_minor_num), MINOR(major_minor_num));

    cdac_dev_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(cdac_dev_class)) {
        pr_err("Failed to create device class.\n");
        unregister_chrdev_region(major_minor_num, 1);
        return PTR_ERR(cdac_dev_class);
    }

    cdev_init(&cdac_cdev, &cdac_fops);
    cdac_cdev.owner = THIS_MODULE;
    ret = cdev_add(&cdac_cdev, major_minor_num, 1);
    if (ret < 0) {
        pr_err("Failed to add cdev. Error: %d\n", ret);
        class_destroy(cdac_dev_class);
        unregister_chrdev_region(major_minor_num, 1);
        return ret;
    }

    if (IS_ERR(device_create(cdac_dev_class, NULL, major_minor_num, NULL, DEVICE_NAME))) {
        pr_err("Failed to create device node.\n");
        cdev_del(&cdac_cdev);
        class_destroy(cdac_dev_class);
        unregister_chrdev_region(major_minor_num, 1);
        return -1;
    }

    pr_info("Device driver initialized successfully. Device node: /dev/%s\n",
           DEVICE_NAME);
    return 0;
}

static void __exit cdac_dev_exit(void) {
    struct cdac_employee_node *current_node, *temp_node;

    pr_info("Exiting CDAC Employee Device Driver...\n");

    list_for_each_entry_safe(current_node, temp_node, &employee_list, list) {
        pr_info("Freeing employee: Name=%s, ID=%d\n",
               current_node->data.empname, current_node->data.empid);
        list_del(&current_node->list);
        kfree(current_node);
    }

    device_destroy(cdac_dev_class, major_minor_num);
    class_destroy(cdac_dev_class);
    cdev_del(&cdac_cdev);
    unregister_chrdev_region(major_minor_num, 1);

    pr_info("Device driver exited successfully.\n");
}

module_init(cdac_dev_init);
module_exit(cdac_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gemini");
MODULE_DESCRIPTION("CDAC Device Driver for Employee Records");
MODULE_VERSION("1.0");

