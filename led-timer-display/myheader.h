#ifndef MYHEADER_H
#define MYHEADER_H

#include <linux/ioctl.h>

#define ioc 'k'
#define SEND_INT _IOW(ioc, 1, unsigned int)
#define START_TIMER_UP _IO(ioc, 2)
#define START_TIMER_DOWN _IO(ioc, 3)
#define STOP_TIMER _IO(ioc, 4)

#endif
