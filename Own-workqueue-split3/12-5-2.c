#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/module.h> 
#include "myheader.h"

irq_handler_t button_handler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
    pr_info("In button press interrupt!\n");

    pr_info("Scheduling work...\n");
    queue_work(my_work_queue, &my_work);

    pr_info("Exiting interrupt handler\n");

    return (irq_handler_t) IRQ_HANDLED;
}

MODULE_LICENSE("GPL"); 
