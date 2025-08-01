/* Basic kernel module skeleton */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt // kmod is a macro 

#include <linux/module.h> //
#include <linux/init.h> //

static char string[] = "Welcome";// in kernal space
static int num = -100;//in kernal space

static int __init my_mod_init(void)// typically prints something
{
	pr_alert("Hello Dhanashree %s!\n", string); // will be seen in dmesg
	return 0; // for indicating successfull excecution of program
}

static void __exit my_mod_exit(void)
{
	pr_alert("Goodbye Dhanashree %d!\n", num); // unloading the module is always successfull 
	return;
}

module_init(my_mod_init); // initialization the module registers itself and takes responsibility and further authority for excecution and its effects 
module_exit(my_mod_exit); // 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Hello world module!");
