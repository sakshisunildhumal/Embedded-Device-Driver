#include<stdio.h>
#include<stdlib.h>
#include<math.h> // For sqrt

typedef struct employee_info
{
    char empname[20]; 
    char gender;      
    int empid;        
} info;

typedef struct node
{
    info data;
    struct node *next;
} NODE;
//////////////////////////////////////////////////////////////////////////

int is_prime(int num)
{
    if (num <= 1) 
    {
    	return 0; 
    }
    if (num <= 3) 
    {
    	return 1; 
    }
    
    if (num % 2 == 0 || num % 3 == 0) 
    {
    	return 0; 
    }

    for (int i = 5; i * i <= num; i = i + 6)
    {
        if (num % i == 0 || num % (i + 2) == 0)
        return 0;
    }
    return 1;
}

void createdata(info *empData)
{
    
    empData->empid = (rand() % 99999) + 2; 
}

NODE *createnewnode(NODE **Head)
{
    NODE *t1, *temp;
    t1 = *Head;
    temp = (NODE *) malloc(sizeof(NODE));
    if(temp == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }
    createdata(&(temp->data)); 
    temp->next = NULL;

    if(*Head == NULL)
    {
        *Head = temp;
        temp->next = NULL;
    }
    else
    {
        while(t1->next != NULL)
        {
            t1 = t1->next;
        }
        t1->next = temp;
        temp->next = NULL;
    }
    return temp;
}

//////////////////////////////////////////////////////////////////////////
void insertatEND(NODE **Head);
void insertatBEG(NODE **Head);
void insertatPOS(NODE **Head, int position);
void deleteatend(NODE **Head);
void deleteatBEG(NODE **Head);
void deleteatPOS(NODE **Head, int position);
void display(NODE *Head);
void freeall(NODE **Head);
void node_cnt_nonprime(NODE *Head_non_prime);
void node_cnt_prime(NODE *Head_prime);
void display_prime(NODE *Head_prime);
void display_non_prime(NODE *Head_non_prime);

int main()
{
    NODE *HEAD = NULL;
    NODE *HEAD_PRIME = NULL; 
    NODE *HEAD_NON_PRIME = NULL; 
    NODE* t1;
    int n, nval;
    char choice;
    int position;

    do
    {
        printf("\nWhat do you want to perform?\n1: Insert in the Linked list (End)\n2: Insert in the Linked list (Beginning)\n3: Insert in the Linked list (Specific Position)\n4: Delete from the linked list (End)\n5: Delete from the linked list (Beginning)\n6: Delete from the linked list (Specific Position)\n7: Display the Linked list\n8: Generate user defined random numbers and classify them\n9: Display Prime Linked List\n10: Display Non-Prime Linked List\n11: count for non prime numbers\n12: count for prime numbers\n");
        scanf("%d", &n);

        switch(n)
        {
            case 1:
                insertatEND(&HEAD); 
                break;
            case 2:
                insertatBEG(&HEAD); 
                break;
            case 3:
                printf("Enter the position where you want to insert: ");
                scanf("%d", &position);
                insertatPOS(&HEAD, position); 
                break;
            case 4:
                printf("How many Nodes do you want to delete? \n");
                scanf("%d", &nval);
                for(int i = 0; i < nval; i++)
                {
                    deleteatend(&HEAD);
                }
                break;
            case 5:
                deleteatBEG(&HEAD);
                break;
            case 6:
                printf("Enter the position of the node to delete: ");
                scanf("%d", &position);
                deleteatPOS(&HEAD, position);
                break;
            case 7:
                display(HEAD);
                break;
            case 8:
                printf("Enter the number for which you want to generate the random integers\n");
                scanf("%d", &nval);
                for(int i = 0; i < nval; i++)
                {
                    NODE *newNode = (NODE *) malloc(sizeof(NODE));
                    if (newNode == NULL)
                    {
                        printf("Memory allocation failed for new node\n");
                        break;
                    }
                    createdata(&(newNode->data)); 

                    if (is_prime(newNode->data.empid))
                    {
                        newNode->next = NULL;
                        if (HEAD_PRIME == NULL)
                        {
                            HEAD_PRIME = newNode;
                        }
                        else
                        {
                            NODE *current = HEAD_PRIME;
                            while (current->next != NULL)
                            {
                                current = current->next;
                            }
                            current->next = newNode;
                        }
                    }
                    else
                    {
                        newNode->next = NULL;
                        if (HEAD_NON_PRIME == NULL)
                        {
                            HEAD_NON_PRIME = newNode;
                        }
                        else
                        {
                            NODE *current = HEAD_NON_PRIME;
                            while (current->next != NULL)
                            {
                                current = current->next;
                            }
                            current->next = newNode;
                        }
                    }
                }
                printf("Generated and classified %d numbers into prime and non-prime lists.\n", nval);
                break;
            case 9:
                display_prime(HEAD_PRIME);
                break;
            case 10:
                display_non_prime(HEAD_NON_PRIME);
                break;
                
            case 11:
            	node_cnt_nonprime(HEAD_NON_PRIME);
                break;
                
            case 12:
            	node_cnt_prime(HEAD_PRIME);
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        printf("\nDo you want to continue (Y/N)? ");
        scanf(" %c", &choice);

    } while(choice == 'Y' || choice == 'y');

    freeall(&HEAD);
    freeall(&HEAD_PRIME); 
    freeall(&HEAD_NON_PRIME); 
    printf("Exiting program.\n");
    return 0;
}

void insertatEND(NODE **Head)
{
    NODE *t1;
    NODE *temp;

    temp = (NODE *) malloc(sizeof(NODE));
    if(temp == NULL)
    {
        printf("Memory allocation failed\n");
        return;
    }

    printf("Enter the name of the employee: ");
    scanf(" %19[^\n]", temp->data.empname);

    printf("Enter the Employee ID: ");
    scanf("%d", &temp->data.empid);

    printf("Enter the gender of the employee (M/F): ");
    scanf(" %c", &temp->data.gender);

    temp->next = NULL;

    if(*Head == NULL)
    {
        printf("This is the first node in the linked list.\n");
        *Head = temp;
    }
    else
    {
        t1 = *Head;
        while(t1->next != NULL)
        {
            t1 = t1->next;
        }
        t1->next = temp;
        printf("Node inserted at the end.\n");
    }
}

void insertatBEG(NODE **Head)
{
    NODE *temp;
    temp = (NODE *) malloc(sizeof(NODE));
    if(temp == NULL)
    {
        printf("Memory allocation Failed\n");
        return;
    }

    printf("Enter the name of the employee: ");
    scanf(" %19[^\n]", temp->data.empname);

    printf("Enter the Employee ID: ");
    scanf("%d", &temp->data.empid);

    printf("Enter the gender of the employee (M/F): ");
    scanf(" %c", &temp->data.gender);

    temp->next = NULL;

    if(*Head == NULL)
    {
        printf("This is the first node in the linked list.\n");
        *Head = temp;
    }
    else
    {
        temp->next = *Head;
        *Head = temp;
        printf("Node inserted at the beginning.\n");
    }
}

void insertatPOS(NODE **Head, int position)
{
    NODE *t1, *temp;
    int i;

    if (position < 1) {
        printf("Position cannot be less than 1.\n");
        return;
    }

    temp = (NODE *) malloc(sizeof(NODE));
    if(temp == NULL)
    {
        printf("Memory allocation Failed\n");
        return;
    }

    printf("Enter the name of the employee: ");
    scanf(" %19[^\n]", temp->data.empname);

    printf("Enter the Employee ID: ");
    scanf("%d", &temp->data.empid);

    printf("Enter the gender of the employee (M/F): ");
    scanf(" %c", &temp->data.gender);

    temp->next = NULL;

    if (position == 1)
    {
        temp->next = *Head;
        *Head = temp;
        printf("Node inserted at position 1.\n");
        return;
    }

    t1 = *Head;
    for (i = 1; i < position - 1 && t1 != NULL; i++)
    {
        t1 = t1->next;
    }

    if (t1 == NULL)
    {
        printf("Position out of bounds. Node not inserted.\n");
        free(temp);
    }
    else
    {
        temp->next = t1->next;
        t1->next = temp;
        printf("Node inserted at position %d.\n", position);
    }
}

void deleteatend(NODE **Head)
{
    NODE *t1, *t2;

    if(*Head == NULL)
    {
        printf("There are no more nodes to delete. The list is empty.\n");
        return;
    }
    else if ((*Head)->next == NULL)
    {
        free(*Head);
        *Head = NULL;
        printf("The last node has been deleted. The list is now empty.\n");
        return;
    }
    else
    {
        t1 = *Head;
        while(t1->next != NULL)
        {
            t2 = t1;
            t1 = t1->next;
        }
        t2->next = NULL;
        free(t1);
        printf("Node deleted from the end.\n");
    }
}

void deleteatBEG(NODE **Head)
{
    NODE *t1;
    if(*Head == NULL)
    {
        printf("No nodes to delete in the linked list.\n");
        return;
    }
    else
    {
        t1 = *Head;
        *Head = (*Head)->next;
        free(t1);
        printf("Node deleted from the beginning.\n");
    }
}

void deleteatPOS(NODE **Head, int position)
{
    NODE *t1, *prev;
    int i;

    if (*Head == NULL)
    {
        printf("Linked list is empty. Nothing to delete.\n");
        return;
    }

    if (position < 1) {
        printf("Position cannot be less than 1.\n");
        return;
    }


    if (position == 1)
    {
        t1 = *Head;
        *Head = t1->next;
        free(t1);
        printf("Node deleted from position 1.\n");
        return;
    }

    t1 = *Head;
    prev = NULL;
    for (i = 1; i < position && t1 != NULL; i++)
    {
        prev = t1;
        t1 = t1->next;
    }

    if (t1 == NULL)
    {
        printf("Position out of bounds. Node not deleted.\n");
    }
    else
    {
        prev->next = t1->next;
        free(t1);
        printf("Node deleted from position %d.\n", position);
    }
}

void display(NODE *Head)
{
    NODE *t1;

    if (Head == NULL)
    {
        printf("Linked list is empty. Nothing to display.\n");
        return;
    }

    t1 = Head;
    printf("\n--- Employee Information ---\n");
    while(t1 != NULL)
    {
        printf("Employee Name: %s\n", t1->data.empname);
        printf("Employee ID: %d\n", t1->data.empid);
        printf("Gender: %c\n", t1->data.gender);
        printf("----------------------------\n");
        t1 = t1->next;
    }
}

void display_prime(NODE *Head_prime)
{
    NODE *t1;

    if (Head_prime == NULL)
    {
        printf("Prime Linked list is empty. Nothing to display.\n");
        return;
    }

    t1 = Head_prime;
    printf("\n--- Prime Numbers ---\n");
    while(t1 != NULL)
    {
        printf("Number: %d\n", t1->data.empid);
        printf("---------------------\n");
        t1 = t1->next;
    }
}

void node_cnt_prime(NODE *Head_prime)
{
int cntr = 0;
    NODE *t1;

    if (Head_prime == NULL)
    {
        printf("Prime Linked list is empty. Nothing to display.\n");
        return;
    }

    t1 = Head_prime;
    printf("\n--- Prime Numbers ---\n");
    while(t1 != NULL)
    {
    	cntr++;
        t1 = t1->next;
    }
    printf("The number of prime number from rand() function are %d", cntr);
}

void display_non_prime(NODE *Head_non_prime)
{
    NODE *t1;

    if (Head_non_prime == NULL)
    {
        printf("Non-Prime Linked list is empty. Nothing to display.\n");
        return;
    }

    t1 = Head_non_prime;
    printf("\n--- Non-Prime Numbers ---\n");
    while(t1 != NULL)
    {
        printf("Number: %d\n", t1->data.empid);
        printf("-----------------------\n");
        t1 = t1->next;
    }
}

void node_cnt_nonprime(NODE *Head_non_prime)
{
int cntr = 0;
    NODE *t1;

    if (Head_non_prime == NULL)
    {
        printf("Non-Prime Linked list is empty. Nothing to display.\n");
        return;
    }

    t1 = Head_non_prime;
    printf("\n--- Non-Prime Numbers ---\n");
    while(t1 != NULL)
    {
    	 cntr++;
        t1 = t1->next;
    }
    printf("The number of non prime number from rand() function are %d", cntr);
}

void freeall(NODE **Head)
{
    NODE* current = *Head;
    NODE* next_node;

    while (current != NULL)
    {
        next_node = current->next;
        free(current);
        current = next_node;
    }
    *Head = NULL;
    printf("All nodes in the linked list have been freed.\n");
}

 /*alter this code such that it sorts the prime and non prime linked list in descending oerder do not alter my variable names and functions only change the logic do not add any of the comments and do not use string library use normal printf scanf so when we display them they appear in descending order use my variable names only do not add yours */
