#ifndef _MYHEADER_H_
#define _MYHEADER_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <errno.h>

#define PRIME_MAGIC 'p'

#define ADD _IOW(PRIME_MAGIC, 1, int)
#define GET _IOR(PRIME_MAGIC, 2, int*)
#define FREEALL _IO(PRIME_MAGIC, 3)

#endif 
