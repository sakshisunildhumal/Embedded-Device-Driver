#include<stdio.h>
#include<math.h>


typedef struct funptrs
{
    float (*add)(float, float);
    float (*substract)(float, float);
    float (*multiply)(float, float);
    float (*divide)(float, float);
    
    float (*sin1)(float);
    float (*cos1)(float);
    float (*tan1)(float);
}fptrs;


float add(float x, float y)
{
	float result = x + y;
	return result;
}

float substract(float x, float y)
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
	float num1, num2, val;
	printf("Enter two numbers for performing Addition, Substraction, Multiplication and Division\n");
	scanf("%f %f", &num1, &num2);

	fptrs f1;
	
    f1.add = add;
    f1.substract = substract;
    f1.multiply = multiply;
    f1.divide = divide;
    f1.sin1 = sin1;
    f1.cos1 = cos1;
    f1.tan1 = tan1;
    
	float result = (f1.add)(num1, num2);
printf("\nResult: Adding %.2f & %.2f = %.2f\n", num1, num2, result);
	result = (f1.substract)(num1, num2);
printf("Result: Substracting %.2f & %.2f = %.2f\n", num1, num2, result);
	result = (f1.multiply)(num1, num2);
printf("Result: Multipying %.2f & %.2f = %.2f\n", num1, num2, result);
	result = (f1.divide)(num1, num2);
printf("Result: Dividing %.2f & %.2f = %.2f\n", num1, num2, result);

	printf("\nEnter a number for which you want to calculate sin, cos, tan\n");
	scanf("%f", &val);
	
	result = (f1.sin1)(val);
	printf("Result: Sin of %.2f is %.2f\n", val, result);
	
	result = (f1.cos1)(val);
	printf("Result: Cos of %.2f is %.2f\n", val, result);
	
	result = (f1.tan1)(val);
	printf("Result: Tan of %.2f is %.2f\n", val, result);
	return 0;
}
