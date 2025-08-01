#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <errno.h>

#include "myheader.h"

#define DEV "/dev/cdac_dev"

int main()
{
    int fd;
    int choice;

    fd = open(DEV, O_RDWR);
    if (fd < 0)
    {
        perror("Cannot open device file");
        exit(EXIT_FAILURE);
    }

    printf("Welcome to the Kernel Timer IOCTL Application!\n");

    while (1)
    {
        printf("\nChoose an option:\n");
        printf("1. Start Kernel Timer\n");
        printf("2. Stop Kernel Timer\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice)
        {
            case 1:
                printf("Attempting to send START_TIMER IOCTL...\n");
                if (ioctl(fd, START_TIMER) < 0)
                {
                    perror("IOCTL START_TIMER failed");
                }
                else
                {
                    printf("START_TIMER IOCTL sent successfully.\n");
                }
                break;

            case 2:
                printf("Attempting to send STOP_TIMER IOCTL...\n");
                if (ioctl(fd, STOP_TIMER) < 0)
                {
                    perror("IOCTL STOP_TIMER failed");
                }
                else
                {
                    printf("STOP_TIMER IOCTL sent successfully.\n");
                }
                break;

            case 3:
                printf("Exiting application.\n");
                close(fd);
                return (EXIT_SUCCESS);

            default:
                printf("Invalid choice. Please enter 1, 2, or 3.\n");
                break;
        }
    }

    close(fd);
    return (EXIT_SUCCESS);
}

