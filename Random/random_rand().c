#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

int main()
{
	struct timeval t;
	int a;
	int i;
	a=gettimeofday(&t,NULL);
	
	srand(t.tv_sec + t.tv_usec);
	
	for(i=0;i<1000;i++)
	{
		printf("%d  ",rand());
	}
	printf("\n");
	
	printf("rand max is %d\n", RAND_MAX);
	
	int random_0_to_100 = (rand() % 100);
	char random_char_a_to_z = (rand() % 26) + 'a';
	char random_char_A_to_Z = (rand() % 26) + 'A';
	printf("Random 0-100: %d\n", random_0_to_100);
	printf("Random 'a'-'z' in integer : %d\n", random_char_a_to_z);
	printf("Random 'a'-'z' in character : %c\n", random_char_a_to_z);
	
	printf("Random 'A'-'Z' in integer : %d\n", random_char_A_to_Z);
	printf("Random 'A'-'Z' in character : %c\n", random_char_A_to_Z);
	
	for(int k = 0; k < 1000; k++)
    	{
        	printf("%c", (rand() % 26) + 'A');
		for(int j = 0; j < 9; j++)
		{
		    printf("%c", (rand() % 26) + 'a');
		}
      		printf("   ");
   	 }

	
	return 0;
	
}
