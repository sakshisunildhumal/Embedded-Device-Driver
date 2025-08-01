#include<stdio.h>
#include<stdlib.h> 

char *to_binary(unsigned int num);
char *to_octal(unsigned int num);
char *to_hexadecimal(unsigned int num);
unsigned int to_decimal(char *, char *, char *);
void reverse_string(char* str, int length);

int main()
{
    int n,k;
    unsigned int num; 
    char *value; 
    char ipstr[35];
    
    while(1)
    {        
        printf("Enter the function you want to perform \n1: Convert to Binary\n2: Convert to Octal\n3: Convert to Hexadecimal\n4: Convert the string back to numbers\n5: Exit\n");
        scanf("%d", &n);

        if(n > 5 || n < 1)
        {
            printf("Enter a valid choice\n");
            continue; 
        }
        
        //printf("Enter a number which you want to convert:\n");
       //scanf("%u", &num);
       
       switch(n)
        {
            case 1:
		    printf("Enter how many values you need from 0\n");
		    scanf("%d", &k);
		    for(num = 0; num < k; num++)
		    {
		        value = to_binary(num);
		        printf("The binary value of %u is %s\n", num, value);
	            } 
            break;
            case 2:
            	    printf("Enter how many values you need from 0\n");
		    scanf("%d", &k);
            	    for(num = 0; num < k; num++)
		    {
		        value = to_octal(num);
		        printf("The octal value of %u is %s\n", num, value);
		    } 
            break;
            case 3:
                    printf("Enter how many values you need from 0\n");
		    scanf("%d", &k);
            	    for(num = 0; num < k; num++)
		    {
		        value = to_hexadecimal(num);
		        printf("The hexadecimal value of %u is %s\n", num, value); 
		    }
            break;
            case 4:
            	printf("Enter the binary, octal or hexadecimal string to convert to decimal:\n");
		scanf("%s", ipstr); 
		num = to_decimal(ipstr, ipstr, ipstr); 
		printf("The decimal value is: %u\n", num); 
		num = to_decimal(ipstr, ipstr, ipstr);
            break;
            case 5:
    		exit(0);
            break;
        }
    }
    return 0;
}

char *to_binary(unsigned int num) 
{
    static char binarystr[35]; 
    int i = 0;

    if (num == 0) 
    {
        binarystr[0] = '0';
        binarystr[1] = 'b';
        binarystr[2] = '0';
        binarystr[3] = '\0';
        return binarystr;
    }

    while (num > 0) 
    {
        binarystr[i++] = (num % 2) + '0';
        num /= 2;
    }

    binarystr[i++] = 'b';
    binarystr[i++] = '0';
    binarystr[i] = '\0';

    reverse_string(binarystr, i);
    return binarystr;
}

char *to_octal(unsigned int num) 
{
    static char octalstr[13]; 
    int i = 0;

    if (num == 0) 
    {
        octalstr[0] = '0';
        octalstr[1] = 'o';
        octalstr[2] = '0';
        octalstr[3] = '\0';
        return octalstr;
    }

    while (num > 0) 
    {
        octalstr[i++] = (num % 8) + '0';
        num /= 8;
    }

    octalstr[i++] = 'o';
    octalstr[i++] = '0';
    octalstr[i] = '\0';

    reverse_string(octalstr, i);
    return octalstr;
}

char *to_hexadecimal(unsigned int num) 
{
    static char hexstr[11]; 
    int i = 0;
    int rem;

    if (num == 0) 
    {
        hexstr[0] = '0';
        hexstr[1] = 'x';
        hexstr[2] = '0';
        hexstr[3] = '\0';
        return hexstr;
    }

    while (num > 0) 
    {
        rem = num % 16;
        if (rem < 10) 
        {
            hexstr[i++] = rem + '0';
        } else 
        {
            hexstr[i++] = rem + 'a' - 10;
        }
        num /= 16;
    }

    hexstr[i++] = 'x';
    hexstr[i++] = '0';
    hexstr[i] = '\0';

    reverse_string(hexstr, i);
    return hexstr;
}

void reverse_string(char* str, int length) 
{
    int start = 0;
    int end = length - 1;
    char temp;
    while (start < end) 
    {
        temp = *(str + start);
        *(str + start) = *(str + end);
        *(str + end) = temp;
        start++;
        end--;
    }
    /*////
    if(binstr != NULL && binstr[0] == '0' && binstr[1] == 'b')
    {
    	currstr = binstr + 2;
    	for(i = 0; currstr[i] != '\0'; i++)
    	{
    		decimal_val = decimal_val * 2;
    		if(currstr[i] == 1)
    		{
    			decima;_val += 1;
    		}
    		else if(currestr[i] != '0')
    		{
    			printf("Invalid binary digit: %c\n", currstr[i]);
    		}
    	}
    }
    ////*/
}

unsigned int to_decimal(char *binstr, char *octstr, char *hexstr) 
{
    unsigned int decimal_val = 0;
    int i;
    int digit_val;
    char *currstr = NULL;

    if (binstr != NULL && binstr[0] == '0' && binstr[1] == 'b') 
    {
        currstr = binstr + 2; 
        for (i = 0; currstr[i] != '\0'; i++) 
        {
            decimal_val = decimal_val * 2;
            if (currstr[i] == '1') 
            {
                decimal_val += 1;
            } 
            else if (currstr[i] != '0') 
            {
                printf("Invalid binary digit: %c\n", currstr[i]);
                return 0;
            }
        }
        return decimal_val;

    } else if (octstr != NULL && octstr[0] == '0' && octstr[1] == 'o') 
    {
        currstr = octstr + 2; 
        for (i = 0; currstr[i] != '\0'; i++) 
        {
            decimal_val = decimal_val * 8;
            if (currstr[i] >= '0' && currstr[i] <= '7') 
            {
                decimal_val += (currstr[i] - '0');
            } 
            else 
            {
                printf("Invalid octal digit: %c\n", currstr[i]);
                return 0;
            }
        }
        return decimal_val;

    } 
    else if (hexstr != NULL && hexstr[0] == '0' && hexstr[1] == 'x') 
    {
        currstr = hexstr + 2; 
        for (i = 0; currstr[i] != '\0'; i++) 
        {
            decimal_val = decimal_val * 16;
            if (currstr[i] >= '0' && currstr[i] <= '9') 
            {
                digit_val = currstr[i] - '0';
            } 
            else if (currstr[i] >= 'a' && currstr[i] <= 'f') 
            {
                digit_val = currstr[i] - 'a' + 10;
            } 
            else if (currstr[i] >= 'A' && currstr[i] <= 'F') 
            {
                digit_val = currstr[i] - 'A' + 10;
            } 
            else 
            {
                printf("Invalid hexadecimal digit: %c\n", currstr[i]);
                return 0;
            }
            decimal_val += digit_val;
        }
        return decimal_val;
    }
    
    printf("No valid prefixed string detected.\n");
    return 0;
}
