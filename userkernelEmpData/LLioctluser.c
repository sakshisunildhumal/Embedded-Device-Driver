#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "header.h"

#define DEVICE_FILE_PATH "/dev/cdac_dev"

void createdata(info *empData) 
{
    int i;
    empData->empname[0] = (rand() % 26) + 'A';
    for (i = 1; i < 10; i++) {
        empData->empname[i] = (rand() % 26) + 'a';
    }
    empData->empname[10] = '\0';

    if (rand() % 2 == 0) {
        empData->gender = 'M';
    } else {
        empData->gender = 'F';
    }

    empData->empid = (rand() % 9000) + 1000;
}

int main() {
    int fd;
    int ret;
    int list_size;
    info emp;
    char temp_name[20];
    int i;
    int j;

    printf("--- CDAC Employee Device User Application ---\n");

    fd = open(DEVICE_FILE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device file");
        fprintf(stderr, "Please ensure the kernel module is loaded and the device node '%s' exists with correct permissions.\n",
                DEVICE_FILE_PATH);
        exit(EXIT_FAILURE);
    }
    printf("Successfully opened device file: %s\n", DEVICE_FILE_PATH);

    printf("\n--- Sending 10 employee records ---\n");
    for (i = 0; i < 10; ++i) {
        createdata(&emp);
        emp.empid = 1000 + i;
        
        temp_name[0] = 'I'; temp_name[1] = 'n'; temp_name[2] = 'i'; temp_name[3] = 't';
        temp_name[4] = 'i'; temp_name[5] = 'a'; temp_name[6] = 'l'; temp_name[7] = 'E';
        temp_name[8] = 'm'; temp_name[9] = 'p'; temp_name[10] = '\0';

        for(j = 0; j < 20; j++) {
            emp.empname[j] = temp_name[j];
            if(temp_name[j] == '\0') break;
        }

        emp.gender = (i % 2 == 0) ? 'M' : 'F';

        ret = ioctl(fd, SEND_EMPLOYEE_DATA, &emp);
        if (ret < 0) {
            perror("IOCTL SEND_EMPLOYEE_DATA failed");
        } else {
            printf("Sent employee: Name=%s, ID=%d, Gender=%c\n", emp.empname, emp.empid, emp.gender);
        }
    }

    ret = ioctl(fd, GET_LIST_SIZE, &list_size);
    if (ret < 0) {
        perror("IOCTL GET_LIST_SIZE failed");
    } else {
        printf("\nKernel linked list size after 10 additions: %d\n", list_size);
    }

    printf("Triggering kernel to print list contents to dmesg...\n");
    ret = ioctl(fd, PRINT_LIST);
    if (ret < 0) {
        perror("IOCTL PRINT_LIST failed");
    } else {
        printf("Check 'dmesg' for output\n");
    }

    ret = ioctl(fd, GET_LIST_SIZE, &list_size);
    if (ret < 0) {
        perror("IOCTL GET_LIST_SIZE failed");
    } else {
        printf("\nKernel linked list total size: %d\n", list_size);
    }

    close(fd);
    printf("\nDevice file closed. Application finished.\n");

    return EXIT_SUCCESS;
}

