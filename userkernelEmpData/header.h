#ifndef _MYHEADER_H_
#define _MYHEADER_H_

#include <linux/ioctl.h>

typedef struct employee_info {
    char empname[20];
    char gender;
    int empid;
} info;

#define EMPLOYEE_IOC_MAGIC 'E'
#define SEND_EMPLOYEE_DATA _IOW(EMPLOYEE_IOC_MAGIC, 1, struct employee_info)
#define GET_LIST_SIZE      _IOR(EMPLOYEE_IOC_MAGIC, 2, int)
#define PRINT_LIST         _IO(EMPLOYEE_IOC_MAGIC, 3)

#endif

