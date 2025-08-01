#ifndef _MYHEADER_H_
#define _MYHEADER_H_

#include <linux/ioctl.h>

#define Myioc 'k'

#define SEND_INT _IOW(Myioc, 0, unsigned int)

#endif
