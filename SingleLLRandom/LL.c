//only linked list with duplicate check and random data generator // duplicate not working properly
#include<stdio.h>
#include<stdlib.h> 

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

void createdata(info *empData)
{
	//
	int found = 0; 
	 empData->empname[0] = (rand() % 26) + 'A'; 
	for (int i = 1; i < 10; i++) 
	{
		empData->empname[i] = (rand() % 26) + 'a'; 
	}
	empData->empname[10] = '\0';  
	if (rand() % 2 == 0)
	{
		empData->gender = 'M';
	}
	else
	{
		empData->gender = 'F';
	}
	
	empData->empid = (rand() % 9000) + 1000;

    do
    {
        empData->empid = (rand() % 9000) + 1000;
       
        for(int i = 0; i <= empData->empid; i++)
        {
            if(empData->empid == i)
            {
                found = 1;
                printf("Duplicate id found\n");
                break;
            }
        }
    } while (found);
	
}
NODE *createnewnode(NODE **Head)
{
		//temp->data = //from rand()
		//take data from rand() into linked list
	NODE *t1, *temp;
	t1 = *Head;
	temp = (NODE *) malloc(sizeof(NODE));
	if(temp == NULL)
	{
		printf("Memory allocation failed\n");
		return NULL;
	}
	createdata(&(temp->data)); // taking data from rand() wala function into new nodes data part
	temp->next = NULL; // new nodes next part should be null
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

int main()
{
    NODE *HEAD = NULL; NODE* t1; 
    int n, nval;
    char choice;
    int position;

    do
    {
        printf("\nWhat do you want to perform?\n1: Insert in the Linked list (End)\n2: Insert in the Linked list (Beginning)\n3: Insert in the Linked list (Specific Position)\n4: Delete from the linked list (End)\n5: Delete from the linked list (Beginning)\n6: Delete from the linked list (Specific Position)\n7: Display the Linked list\n8: New node data using rand() function\n");
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
            	printf("How many Nodes do you want to create? \n");
            	scanf("%d", &nval);
            	for(int i = 0; i < nval; i++)
            	{
            		t1 = createnewnode(&HEAD);
    		}
            break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        printf("\nDo you want to continue (Y/N)? ");
        scanf(" %c", &choice); 

    } while(choice == 'Y' || choice == 'y');

    freeall(&HEAD);
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
    {
        printf("Employee Name: %s\n", t1->data.empname);
        printf("Employee ID: %d\n", t1->data.empid);
        printf("Gender: %c\n", t1->data.gender);
        printf("----------------------------\n");
        t1 = t1->next;
    }
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

