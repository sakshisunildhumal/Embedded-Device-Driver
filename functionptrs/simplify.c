#include <stdio.h>
#include <math.h>

typedef struct funptrs
{
    float (*binary_op)(float, float);
    float (*unary_op)(float);

} fptrs;

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
    if(y == 0)
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

float sin1(float x)
{
    float result = sin(x);
    return result;
}

float cos1(float x)
{
    float result = cos(x);
    return result;
}

float tan1(float x)
{
    float result = tan(x);
    return result;
}

int main()
{
    float num1, num2, val, result;
    int choice_binary, choice_unary;
    fptrs f1; char choice;
do
{
    printf("Enter two numbers for binary operations:\n");
    scanf("%f %f", &num1, &num2);

    printf("Choose binary operation:\n");
    printf("1. Add\n");
    printf("2. Subtract\n");
    printf("3. Multiply\n");
    printf("4. Divide\n");
    scanf("%d", &choice_binary);

    switch (choice_binary)
    {
        case 1:
            f1.binary_op = add;
            result = (f1.binary_op)(num1, num2);
            printf("\nResult: Adding %.2f & %.2f = %.2f\n", num1, num2, result);
            break;
        case 2:
            f1.binary_op = subtract;
            result = (f1.binary_op)(num1, num2);
            printf("Result: Subtracting %.2f & %.2f = %.2f\n", num1, num2, result);
            break;
        case 3:
            f1.binary_op = multiply;
            result = (f1.binary_op)(num1, num2);
            printf("Result: Multiplying %.2f & %.2f = %.2f\n", num1, num2, result);
            break;
        case 4:
            f1.binary_op = divide;
            result = (f1.binary_op)(num1, num2);
            printf("Result: Dividing %.2f & %.2f = %.2f\n", num1, num2, result);
            break;
        default:
            printf("Invalid binary operation choice.\n");
            break;
    }

    printf("\nEnter a number for unary operations:\n");
    scanf("%f", &val);

    printf("Choose unary operation:\n");
    printf("1. Sin\n");
    printf("2. Cos\n");
    printf("3. Tan\n");
    scanf("%d", &choice_unary);

    switch (choice_unary)
    {
        case 1:
            f1.unary_op = sin1;
            result = (f1.unary_op)(val);
            printf("\nResult: Sin of %.2f is %.2f\n", val, result);
            break;
        case 2:
            f1.unary_op = cos1;
            result = (f1.unary_op)(val);
            printf("Result: Cos of %.2f is %.2f\n", val, result);
            break;
        case 3:
            f1.unary_op = tan1;
            result = (f1.unary_op)(val);
            printf("Result: Tan of %.2f is %.2f\n", val, result);
            break;
        default:
            printf("Invalid unary operation choice.\n");
            break;
    }
    
    printf("Do you want to continue?\n");
    scanf("%s", &choice);
}while(choice == 'Y' || choice == 'y');
    return 0;
}
