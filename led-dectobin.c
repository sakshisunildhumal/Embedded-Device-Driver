#define pr_fmt(fmt)    KBUILD_MODNAME ": " fmt

#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>       
#include <linux/kobject.h>    
#include <linux/kthread.h>    
#include <linux/delay.h>      

static struct kobject *kobj_ref;         
static struct task_struct *task;         
                             
static unsigned int gpioLED1 = 60; //LSB
static unsigned int gpioLED2 = 49; //MSB2
static unsigned int gpioLED3 = 48; //MSB1
static unsigned int gpioLED4 = 47; //MSB
            
static unsigned int received_num = 0; 
module_param(received_num, uint, S_IRUGO | S_IWUSR);   
MODULE_PARM_DESC(received_num, "Enter an unsigned int to display on LED ladder betn 0 to 15"); 

static ssize_t num_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
   return sprintf(buf, "%u\n", received_num);
}

static ssize_t num_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
   unsigned int temp;             

   if (sscanf(buf, "%u", &temp) == 1) 
   { 
       received_num = temp; // Update the number to be displayed
   }
   pr_info("Received number set to %u\n", received_num);

   return count; // Return count of bytes processed to keep the kernel happy that something was received and stored successfully
}

static struct kobj_attribute num_attr = __ATTR(value, 0664, num_show, num_store); // Renamed period_attr to num_attr

static struct attribute *led_attrs[] = 
{
   &num_attr.attr,                  
   NULL,
};

static struct attribute_group attr_group = 
{
   .name  = "DHleds", 
   .attrs = led_attrs,                 
};

static int display_binary_leds(void *arg)
{
   pr_info("LED display thread started.\n");

   while(!kthread_should_stop()){         
       // Set LED1 based on the 0th bit of received_num
       gpio_set_value(gpioLED1, (received_num & (1 << 0)) ? 1 : 0);
       // Set LED2 based on the 1st bit
       gpio_set_value(gpioLED2, (received_num & (1 << 1)) ? 1 : 0);
       // Set LED3 based on the 2nd bit
       gpio_set_value(gpioLED3, (received_num & (1 << 2)) ? 1 : 0);
       // Set LED4 based on the 3rd bit
       gpio_set_value(gpioLED4, (received_num & (1 << 3)) ? 1 : 0);
       msleep(100); 
   }

   pr_info("LED display thread exiting\n");
   return 0;
}

static int __init my_init(void){
   int value = 0;

   // Removed sprintf(ledName, "led%d", gpioLED);
   pr_info("Init'ing GPIO LEDs...\n");

   kobj_ref = kobject_create_and_add("cdac_led", NULL); 
   if(!kobj_ref){
       pr_err("Failed to create kobject\n");
       return -1;
   }

   value = sysfs_create_group(kobj_ref, &attr_group);
   if(value) {
       pr_err("Failed to create sysfs group\n");
       goto r_sys_grp;
   }

   // Request all four GPIOs
   value = gpio_request(gpioLED1, "sysfs_led1");         
   if (value < 0) {
       pr_err("Error requesting gpioLED1!\n");
       goto r_gpio_req1;
   } 
   value = gpio_request(gpioLED2, "sysfs_led2");         
   if (value < 0) {
       pr_err("Error requesting gpioLED2!\n");
       goto r_gpio_req2;
   }
   value = gpio_request(gpioLED3, "sysfs_led3");         
   if (value < 0) {
       pr_err("Error requesting gpioLED3!\n");
       goto r_gpio_req3;
   }
   value = gpio_request(gpioLED4, "sysfs_led4");         
   if (value < 0) {
       pr_err("Error requesting gpioLED4!\n");
       goto r_gpio_req4;
   }

   // Setting direction and exporting all GPIOs
   gpio_direction_output(gpioLED1, 0); // initial should be off state
   gpio_export(gpioLED1, false); 
   
   gpio_direction_output(gpioLED2, 0); 
   gpio_export(gpioLED2, false);
   
   gpio_direction_output(gpioLED3, 0); 
   gpio_export(gpioLED3, false);
   
   gpio_direction_output(gpioLED4, 0); 
   gpio_export(gpioLED4, false);

   task = kthread_run(display_binary_leds, NULL, "LED display thread"); 
   if(IS_ERR(task))
   {                                 
       pr_err("Failed to create LED display task\n");
       goto r_task;
   }

   return 0;

r_task:
   gpio_set_value(gpioLED4, 0);
   gpio_unexport(gpioLED4);
   gpio_free(gpioLED4);
   
r_gpio_req4:
   gpio_set_value(gpioLED3, 0);
   gpio_unexport(gpioLED3);
   gpio_free(gpioLED3);
   
r_gpio_req3:
   gpio_set_value(gpioLED2, 0);
   gpio_unexport(gpioLED2);
   gpio_free(gpioLED2);
   
r_gpio_req2:
   gpio_set_value(gpioLED1, 0);
   gpio_unexport(gpioLED1);
   gpio_free(gpioLED1);
   
r_gpio_req1:
   sysfs_remove_group(kobj_ref, &attr_group);
   
r_sys_grp:
   kobject_put(kobj_ref);
   return -1;

}

static void __exit my_exit(void)
{
   kthread_stop(task);

   // Turn off and free all LEDs
   gpio_set_value(gpioLED1, 0);          
   gpio_unexport(gpioLED1);             
   gpio_free(gpioLED1);                 
   pr_info("GPIO %d freed up\n", gpioLED1);

   gpio_set_value(gpioLED2, 0);          
   gpio_unexport(gpioLED2);             
   gpio_free(gpioLED2);                 
   pr_info("GPIO %d freed up\n", gpioLED2);

   gpio_set_value(gpioLED3, 0);          
   gpio_unexport(gpioLED3);             
   gpio_free(gpioLED3);                 
   pr_info("GPIO %d freed up\n", gpioLED3);

   gpio_set_value(gpioLED4, 0);          
   gpio_unexport(gpioLED4);             
   gpio_free(gpioLED4);                 
   pr_info("GPIO %d freed up\n", gpioLED4);

   sysfs_remove_group(kobj_ref, &attr_group);   
   kobject_put(kobj_ref);                 

   printk(KERN_INFO "Good bye from %s!\n", KBUILD_MODNAME);
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("CDAC EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("A simple LED driver using GPIOs for binary display");
