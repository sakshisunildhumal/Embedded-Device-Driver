#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "myheader.h"

int main()
{
    int fd;
    unsigned int num;
    int choice;

    fd = open("/dev/DHdevice", O_RDWR);
    if (fd == -1) 
    {
        perror("Error opening the device file /dev/DHdevice");
        return 1;
    }

    do {
        printf("\nMenu:\n");
        printf("1. Set a specific number (0-15)\n");
        printf("2. Start Counter Up (0-15)\n");
        printf("3. Start Counter Down (15-0)\n");
        printf("4. Stop Counter\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) 
        {
            fprintf(stderr, "Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }

        switch (choice) 
        {
        case 1:
            printf("Enter an unsigned integer between 0 to 15: ");
            if (scanf("%u", &num) != 1) 
            {
                fprintf(stderr, "Please enter an unsigned integer.\n");
                while (getchar() != '\n');
                continue;
            }
            if (ioctl(fd, SEND_INT, &num) == -1) 
            {
                perror("Error sending value via IOCTL\n");
            } else {
                printf("Successfully sent %u to the kernel\n", num);
            }
            break;
        case 2:
            if (ioctl(fd, START_TIMER_UP, 0) == -1) 
            {
                perror("Error sending START_TIMER_UP via IOCTL\n");
            } else {
                printf("Sent START_TIMER_UP command to the kernel\n");
            }
            break;
        case 3:
            if (ioctl(fd, START_TIMER_DOWN, 0) == -1) 
            {
                perror("Error sending START_TIMER_DOWN via IOCTL\n");
            } else {
                printf("Sent START_TIMER_DOWN command to the kernel\n");
            }
            break;
        case 4:
            if (ioctl(fd, STOP_TIMER, 0) == -1) 
            {
                perror("Error sending STOP_TIMER via IOCTL\n");
            } else {
                printf("Sent STOP_TIMER command to the kernel\n");
            }
            break;
        case 5:
            printf("Exiting application.\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 5);

    close(fd);
    return 0;
}
