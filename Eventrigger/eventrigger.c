// DHANASHREE 1ST CODE
#define pr_fmt(fmt)     KBUILD_MODNAME ": " fmt
#define SIZE 10

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

// instantiation with default, overriden by insmod if any
static int myint = 100;
static long myarr[SIZE] = {0};
static int myint_callback = 200;
static int count = 0;

// registration with the kernal and giving permissions
module_param(myint, int, S_IRUGO | S_IWUSR);
module_param_array(myarr, long, &count, S_IRUGO | S_IWUSR);

// module_param(myint_callback, int, S_IRUGO | S_IWUSR);

// create MODULE display strings for modinfo
MODULE_PARM_DESC(myint, "This is an integer variable");
MODULE_PARM_DESC(myarr, "This is an array variable of longs");
MODULE_PARM_DESC(myint_callback, "This is an integer variable");
//////////////////////////////////////////////////////////////

int notify_on_set(const char *val, const struct kernel_param *kp)
{
    int ans = param_set_int(val, kp);
    if (ans==0)
    {
        pr_info("Callback function %s called\n", __func__);
        pr_info("New value of myint_callback = %d\n", myint_callback);
        return 0;
    }
    return -1;
}

const struct kernel_param_ops myops =
{
    .set = &notify_on_set,      // my function - overrides default
    .get = &param_get_int,      // kernel's function - default
};

// permisssions: all can read, only USR (root) can write/edit
module_param_cb(myint_callback, &myops, &myint_callback, S_IRUGO | S_IWUSR);

// create MODULE DESC strings for modinfo
MODULE_PARM_DESC(myint_callback, "This is a int variable with callback");


//////////////////////////////////////////////////////////////

static int __init my_mod_init(void)
{
    int i;
    pr_info("Hello from my new program!\n");
    pr_info("myint = %d, myint_callback=%d\n", myint, myint_callback);
    pr_info("For myarr, we got %d entries\n", count);
    for (i=0;i<count;i++)
    {
        pr_info("myarr[%d]=%ld", i, myarr[i]);
    }
    return 0;
}

static void __exit my_mod_exit(void)
{
    pr_info("Goodbye Exiting from the code!\n");
    return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module with param callback support!");
