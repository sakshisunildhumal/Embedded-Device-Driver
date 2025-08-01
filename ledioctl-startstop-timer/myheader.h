#ifndef MYHEADER_H
#define MYHEADER_H

#include <linux/ioctl.h>

#define ioc 'k'
#define START_TIMER_UP _IO(ioc, 1)
#define START_TIMER_DOWN _IO(ioc, 2)
#define STOP_TIMER _IO(ioc, 3)

#endif

