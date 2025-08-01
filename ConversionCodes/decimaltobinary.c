#include <stdio.h>

int main()
{
    unsigned int num;
    int i;
    int nonzero = 1;

    printf("Enter an unsigned integer from 0 to 100: \n");
    scanf("%u", &num);

    if (num > 100)
    {
        printf("Input out of range 1 to 100\n");
        return 1;
    }

    printf("Binary value is: ");

    if (num == 0) 
    {
        printf("0");
    } 
    else 
    {
        for (i = 7; i >= 0; i--)
        {
            if (((num >> i) & 1) == 1)
            {
                nonzero = 0;
            }
            if (nonzero == 0)
            {
                if ((num >> i) & 1)
                {
                    printf("1");
                } else {
                    printf("0");
                }
            }
        }
    }
    printf("\n");

    return 0;
}
