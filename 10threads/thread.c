/* Kernel thread basics */

#define pr_fmt(fmt)	KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/kthread.h>

#define TIME_INTVL	(1000)	
#define MAX_THREADS 10


static struct task_struct *threads[MAX_THREADS];
static char thread_names[MAX_THREADS][16];
static int thread_ids[MAX_THREADS]={0};
static int num_threads = 1; 
module_param(num_threads, int, 0);
MODULE_PARM_DESC(num_threads, "Number of threads to create (max 10)");

int thread_func(void *pv)
{
	int thread_id = *(int *)pv;
	int i = 0;

	while (!kthread_should_stop())
	{
		pr_info("From thread %s, i=%d\n", thread_names[thread_id], i);
		msleep(TIME_INTVL * (thread_id + 1)); 
	}
	return 0;
}

static int __init my_mod_init(void)
{
	int i;

	if (num_threads < 1 || num_threads > MAX_THREADS)
	{
		pr_err("Invalid number of threads %d. Enter between 1 and %d.\n",
		       num_threads, MAX_THREADS);
		return 0;
	}

	pr_info("Hello world from %s!\n", KBUILD_MODNAME);

	for (i = 0; i < num_threads; i++)
	{
		thread_ids[i] = i;
		snprintf(thread_names[i], sizeof(thread_names[i]), "Thread-%d", i + 1);
		threads[i] = kthread_run(thread_func, &thread_ids[i], thread_names[i]);
		if (threads[i])
		{
			pr_info("Thread %s created!\n", thread_names[i]);
		}
		else
		{
			pr_err("Cannot create thread %s\n", thread_names[i]);
			goto r_thread2;
		}
	}
	return 0;

r_thread2:
	while (--i >= 0)
		kthread_stop(threads[i]);
r_thread1:
	return -1;
}

static void __exit my_mod_exit(void)
{
	int i;
	for (i = 0; i < num_threads; i++)
		kthread_stop(threads[i]);
	pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);
	return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Module to demo kernel threads!");


