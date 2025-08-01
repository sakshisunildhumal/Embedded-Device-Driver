#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "myheader.h"

#define DEVICE_PATH "/dev/cdac_dev"

int main()
{
    int fd;
    int choice;
    unsigned int cmd;

    fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device");
        return EXIT_FAILURE;
    }

    printf("LED TIMER\n");

    while (1) {
        printf("\nChoose an option:\n");
        printf("1. Start Timer (Count towards Up)\n");
        printf("2. Start Timer (Count towards Down)\n");
        printf("3. Stop Timer\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                cmd = START_TIMER_UP;
                printf("Sending START_TIMER_UP command...\n");
                break;
            case 2:
                cmd = START_TIMER_DOWN;
                printf("Sending START_TIMER_DOWN command...\n");
                break;
            case 3:
                cmd = STOP_TIMER;
                printf("Sending STOP_TIMER command...\n");
                break;
            case 4:
                printf("Exiting the code\n");
                close(fd);
                return EXIT_SUCCESS;
            default:
                printf("Invalid choice. Please enter 1, 2, 3, or 4.\n");
                continue;
        }

        if (ioctl(fd, cmd) < 0) {
            perror("Failed to send ioctl command");
        } else {
            printf("IOCTL command sent successfully.\n");
        }
    }

    close(fd);
    return EXIT_SUCCESS;
}

