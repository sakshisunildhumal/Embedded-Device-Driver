#include <stdio.h>
#include<stdlib.h>
#include "myheader.h" 

int main() 
{
    int device;
    int choice;
    int num;
    int *primefound = NULL;
    int primecntr = 0;

    device = open("/dev/cdac_dev", O_RDWR);
    if (device < 0) 
    {
        printf("Failed to open the device");
        return 1;
    }

    srand(time(NULL)); 

    do 
    {
        printf("\nWhat do you want to perform?\n");
        printf("1: Generate random numbers using rand() function and send to kernel\n2: Display Prime Numbers from kernel\n3: Free all\n4:Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) 
        {
            case 1:
                printf("Enter the number of random integers to generate and send: ");
                scanf("%d", &num);
                for (int i = 0; i < num; i++) 
                {
                    int random_num = (rand() % 99999) + 2; 
                    if (ioctl(device, PRIME_ADD_NUMBER, &random_num) < 0) 
                    {
                        printf("IOCTL PRIME_ADD_NUMBER failed");
                    }
                }
                printf("Generated and sent %d numbers to the kernel\n", num);
                break;

            case 2:
                
                primecntr = ioctl(device, GET, NULL); 
                if (primecntr < 0) 
                {
                    printf("IOCTL GET (count) failed");
                    break;
                }
                if (primecntr == 0) 
                {
                    printf("No prime numbers in the kernel list to display.\n");
                    break;
                }

                primefound = (int *)malloc(primecntr * sizeof(int));
                if (primefound == NULL) 
                {
                    printf("Failed to allocate memory for prime numbers");
                    break;
                }

                if (ioctl(device, GET, primefound) < 0) 
                {
                    printf("IOCTL GET function failed");
                    free(primefound);
                    primefound = NULL;
                    break;
                }

                printf("\n--- Prime Numbers from Kernel (Descending Order) ---\n");
                for (int i = 0; i < primecntr; i++) 
                {
                    printf("Number: %d\n", primefound[i]);
                    printf("---------------------\n");
                }
                free(primefound);
                primefound = NULL;
                break;

            case 3:
                if (ioctl(device, PRIME_FREE_ALL) < 0) 
                {
                    printf("IOCTL PRIME_FREE_ALL failed");
                } 
                else 
                {
                    printf("All prime numbers in the kernel list have been freed\n");
                }
                break;

            case 4:
                printf("Exiting program\n");
                break;

            default:
                printf("Invalid choice Enter a valid number\n");
                break;
        }
    } while (choice != 4);

    close(device);
    return 0;
}
