#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "myheader.h" 

struct Node {
    int data;
    struct Node *next;
};

void insertNode(struct Node **headref, int data) {
    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (!newNode) {
        perror("malloc failed");
        return;
    }
    newNode->data = data;
    newNode->next = NULL;

    if (*headref == NULL) {
        *headref = newNode;
    } else {
        struct Node *current = *headref;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

void printList(struct Node *head) {
    struct Node *current = head;
    if (current == NULL) {
        printf("Empty\n");
        return;
    }
    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void freeList(struct Node *head) {
    struct Node *current = head;
    struct Node *next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    int filedesc;
    int choice;
    int val;
    int numcount = 0;
    int upperlimit = 0;
    int lowerlimit = 0;
    int *readbuf = NULL;
    int i;
    struct Node *positiveList = NULL;
    struct Node *negativeList = NULL;

    filedesc = open("/dev/randomdev", O_RDWR);
    if (filedesc < 0) {
        perror("Failed to open device file");
        return 1;
    }

    do {
        printf("\nMenu:\n");
        printf("1. Set number of random numbers\n");
        printf("2. Set upper limit\n");
        printf("3. Set lower limit\n");
        printf("4. Generate and process numbers\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter number of random numbers to generate: ");
                scanf("%d", &val);
                if (ioctl(filedesc, SET_COUNT, &val) < 0) {
                    perror("IOCTL SET_COUNT failed");
                } else {
                    numcount = val;
                }
                break;
            case 2:
                printf("Enter upper limit: ");
                scanf("%d", &val);
                if (ioctl(filedesc, SET_UPPER, &val) < 0) {
                    perror("IOCTL SET_UPPER failed (check limits)");
                } else {
                    upperlimit = val;
                }
                break;
            case 3:
                printf("Enter lower limit: ");
                scanf("%d", &val);
                if (ioctl(filedesc, SET_LOWER, &val) < 0) {
                    perror("IOCTL SET_LOWER failed (check limits)");
                } else {
                    lowerlimit = val;
                }
                break;
            case 4:
                if (numcount <= 0) {
                    printf("Please set number of random numbers (Option 1) first.\n");
                    break;
                }
                if (upperlimit == 0 && lowerlimit == 0) {
                     printf("Using default limits (100, -100) or previously set values.\n");
                }

                if (readbuf != NULL) {
                    free(readbuf);
                    readbuf = NULL;
                }
                readbuf = (int *)malloc(numcount * sizeof(int));
                if (!readbuf) {
                    perror("malloc failed for readbuf");
                    break;
                }

                freeList(positiveList);
                freeList(negativeList);
                positiveList = NULL;
                negativeList = NULL;

                printf("Reading %d numbers from kernel...\n", numcount);
                
                lseek(filedesc, 0, SEEK_SET); 
                if (read(filedesc, readbuf, numcount * sizeof(int)) < 0) {
                    perror("Failed to read numbers from device");
                    free(readbuf);
                    readbuf = NULL;
                    break;
                }

                printf("Numbers received and processing:\n");
                for (i = 0; i < numcount; i++) {
                    printf("%d ", readbuf[i]);
                    if (readbuf[i] >= 0) {
                        insertNode(&positiveList, readbuf[i]);
                    } else {
                        insertNode(&negativeList, readbuf[i]);
                    }
                }
                printf("\n");

                printf("Positive numbers list: ");
                printList(positiveList);
                printf("Negative numbers list: ");
                printList(negativeList);

                free(readbuf);
                readbuf = NULL;
                break;
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    } while (choice != 0);

    close(filedesc);
    freeList(positiveList);
    freeList(negativeList);

    return 0;
}

