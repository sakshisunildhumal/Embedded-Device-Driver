#define pr_fmt(fmt)           KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/kdev_t.h>

#define MY_CLASS_NAME         "dev"
#define MY_DEV_NAME         "cdac_dev" 
#define NUM_DEVS            (2)

static dev_t major_minor_num;
static
