#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include<linux/kernel.h>
#include<linux/random.h>

static int __init my_init(void)
{
    int i;
    //int buff[1000]; 
    unsigned int val;
    int num;
    
    pr_info("Inside Init function\n");
    pr_info("Generating random numbers\n");

    for (i = 0; i < 1000; i++) 
    {
        get_random_bytes(&val, sizeof(val));
        num = (int)((val % 201) - 100);
        pr_info("Number %d: %d\n", i + 1, num);
        
        //buff[i++] = num;
    }
    return 0;
}

static void __exit my_exit(void)
{
    pr_info("Exiting the code for generating the random numbers\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("My own workqueue - dynamic example");



















/*
now give this code such that i want to create a user space and a kernal space program in which the user space program accepts 3 inputs using a switch case for 1, 2, 3 such that it will operate in kernel space for the following options 1 is number of random numbers to generate 2 for upper limit of the random numbers to be genrated and 3 for the lower lmit of the random numbers to be generated and then returns the random numbers to the user sace and  stores them in the user space's linked list in such a way that there are two seperate linked lists created one for positive numbers seperate and one for negative numbers and there should be a check that the upper limit and the lower limit should not be equal (eg if upper limit is 100 the lower limit cannot be -100) like the numbers cant be totally equal and also the numbers cannot be positively and negatively equal do not add comments and use simple variable names without underscores in it do not add unnesessary header file use minimalistic do not use string library using normal pr_info like functions */
