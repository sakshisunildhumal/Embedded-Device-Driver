#include "operations.h" 
#include <stdio.h> 

float add(float x, float y) 
{
    float result = x + y;
    return result;
}

float subtract(float x, float y) 
{
    float result = x - y;
    return result;
}

float multiply(float x, float y) 
{
    float result = x * y;
    return result;
}

float divide(float x, float y) 
{
    if (y == 0) 
    {
        printf("Cannot divide by zero\n");
        return 0;
    } 
    else 
    {
        float result = x / y;
        return result;
    }
}
