#ifndef MYHEADER_H
#define MYHEADER_H

#include <linux/ioctl.h>

#define ioc 'k'
#define SEND_INT _IOW(ioc, 1, unsigned int)

#endif 
