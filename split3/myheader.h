#ifndef _MYHEADER_H_
#define _MYHEADER_H_

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>


#define MY_CLASS_NAME    "cdac_cls"
#define MY_DEV_NAME      "cdac_dev"
#define MY_KBUF_SZ       (1024)


extern dev_t dev;
extern struct class *dev_class;
extern struct device *cdevice;
extern struct cdev my_cdev;
extern char *kbuf; 


extern struct file_operations fops;

extern int my_open(struct inode *inode, struct file *file);
extern int my_release(struct inode *inode, struct file *file);
extern ssize_t my_read(struct file *file, char __user *buf, size_t len, loff_t *off);
extern ssize_t my_write(struct file *file, const char __user *buf, size_t len, loff_t *off);

#endif 
