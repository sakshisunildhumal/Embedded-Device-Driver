
#include "myheader.h" 

#define MY_CLASS_NAME    "cdac_cls"
#define MY_DEV_NAME      "cdac_dev"

typedef struct node 
{
    int data;
    struct node *next;
} NODE;

static NODE *primehead = NULL;


static int isprime(int num) 
{
    if (num <= 1) 
    {
        return 0;
    }
    if (num <= 3) 
    {
        return 1;
    }
    if (num % 2 == 0 || num % 3 == 0) 
    {
        return 0;
    }
    for (int i = 5; i * i <= num; i = i + 6) 
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return 0;
    }
    return 1;
}

static void insert(int num) 
{
    NODE *temp = kmalloc(sizeof(NODE), GFP_KERNEL);
    if (!temp) 
    {
        pr_err("Failed to allocate memory for a node\n");
        return;
    }
    temp->data = num;
    temp->next = NULL;

    if (primehead == NULL || num > primehead->data) 
    {
        temp->next = primehead;
        primehead = temp;
    } 
    else 
    {
        NODE *t1 = primehead;
        while (t1->next != NULL && num < t1->next->data) 
        {
            t1 = t1->next;
        }
        temp->next = t1->next;
        t1->next = temp;
    }
}


static void free_prime_list(void) 
{
    NODE *t1 = primehead;
    NODE *next_node;
    while (t1 != NULL) 
    {
        next_node = t1->next;
        kfree(t1);
        t1 = next_node;
    }
    primehead = NULL;
    pr_info("All prime nodes in kernel list have been freed\n");
}

dev_t dev = 0;
static struct class *dev_class;
static struct device *cdevice;
static struct cdev my_cdev;


static int __init my_mod_init(void);
static long my_ioctl(struct file *file, unsigned int val, unsigned long args);
static void __exit my_mod_exit(void);

static long my_ioctl(struct file *file, unsigned int val, unsigned long arg) {
    int num;
    NODE *t1;
    int count = 0;
    int *buff;

    switch (val) 
    {
        case ADD:
            if (copy_from_user(&num, (int *)arg, sizeof(int))) 
            {
                pr_err("Error copying number from user space to kernel space\n");
                return -1;
            }
            if (isprime(num)) 
            {
                insert(num);
                pr_info("Added a prime number: %d\n", num);
            } else 
            {
                pr_info("Discarded a non-prime number: %d\n", num);
            }
            break;

        case GET:
            t1 = primehead;
            while (t1 != NULL) 
            {
                count++;
                t1 = t1->next;
            }
            
            buff = (int *)arg;

            if (count > 0 && buff != NULL) 
            {
                t1 = primehead;
                for (int i = 0; i < count; i++) 
                {
                    if (copy_to_user(&buff[i], &t1->data, sizeof(int))) 		    {
                        pr_err("Error copying prime number to user space\n");
                        return -1;
                    }
                    t1 = t1->next;
                }
            }
            return count; 

        case FREEALL:
            free_prime_list();
            break;

        default:
            pr_err("Undefined command!\n");
            return -1;
    }
    return 0;
}

static struct file_operations fops = 
{
    .owner = THIS_MODULE,
    .unlocked_ioctl = my_ioctl,
};

static int __init my_mod_init(void) 
{
    int ans;

    pr_info("Hello world from %s!\n", KBUILD_MODNAME);
    ans = alloc_chrdev_region(&dev, 0, 1, MY_DEV_NAME);
    if (ans < 0) 
    {
        pr_err("Error in major:minor allotment!\n");
        return -1;
    }
    pr_info("major:minor %d:%d allotted!\n", MAJOR(dev), MINOR(dev));

    // initializing a cdev for function operations
    cdev_init(&my_cdev, &fops);

    ans = cdev_add(&my_cdev, dev, 1);
    if (ans < 0) 
    {
        pr_err("Could not add cdev to the kernel!\n");
        goto r_cdev;
    }

    dev_class = class_create(THIS_MODULE, MY_CLASS_NAME);
    if (IS_ERR(dev_class)) 
    {
        pr_err("Could not create device class %s\n", MY_CLASS_NAME);
        goto r_class;
    }

    cdevice = device_create(dev_class, NULL, dev, NULL, MY_DEV_NAME);
    if (IS_ERR(cdevice)) 
    {
        pr_err("Could not create device %s\n", MY_DEV_NAME);
        goto r_device;
    }
    pr_info("Device %s under class %s created with success\n",
            MY_DEV_NAME, MY_CLASS_NAME);
    return 0;

r_device:
    class_destroy(dev_class);
r_class:
    cdev_del(&my_cdev);
r_cdev:
    unregister_chrdev_region(dev, 1);

    return -1;
}

static void __exit my_mod_exit(void) 
{
    pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);
    free_prime_list(); 
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&my_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("major:minor numbers were freed up\n");
    return;
}

module_init(my_mod_init);
module_exit(my_mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("IOCTL on char device!");
