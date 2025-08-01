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

    fd = open("/dev/DHdevice", O_RDWR);
    if (fd == -1) 
    {
        perror("Error opening the device file /dev/DHdevice");
        return 1;
    }

    printf("Enter an unsigned integer betn 0 to 15 to display on LED ladder: ");
    if (scanf("%u", &num) != 1) 
    {
        fprintf(stderr, "Please enter an unsigned integer.\n");
        close(fd);
        return 1;
    }

    if (ioctl(fd, SEND_INT, &num) == -1) 
    {
        perror("Error sending value via IOCTL\n");
        close(fd);
        return 1;
    }

    printf("Successfully sent %u to the kernel\n", num);

    close(fd);
    return 0;
}
