#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/workqueue.h>
#include <linux/module.h> 
#include "myheader.h"

void my_work_func(struct work_struct *work)
{
    numPresses++;
    pr_info("In %s: numPresses = %d\n", __func__, numPresses);
}
