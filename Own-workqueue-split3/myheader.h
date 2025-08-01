#ifndef MYHEADER_H
#define MYHEADER_H

#include <linux/workqueue.h>
#include <linux/interrupt.h>

extern unsigned int gpioButton;
extern unsigned int irqNumber;
extern unsigned int numPresses;

extern struct workqueue_struct *my_work_queue;
extern struct work_struct my_work;

void my_work_func(struct work_struct *work);
irq_handler_t button_handler(unsigned int irq, void *dev_id, struct pt_regs *regs);

#endif 
