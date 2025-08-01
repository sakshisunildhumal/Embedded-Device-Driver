#ifndef _MYHEADER_H_
#define _MYHEADER_H_

#include <linux/ioctl.h>

// Define the magic number for our IOCTLs
#define ioc 'k'

// Define the IOCTL commands
#define START_TIMER _IO(ioc, 0)
#define STOP_TIMER  _IO(ioc, 1)

// Placeholder for the original IOCTLs if needed, though they won't be used in the final solution
#define MY_HW_READ  _IOR(ioc, 2, unsigned int)
#define MY_HW_WRITE _IOW(ioc, 3, unsigned int)

#endif 
