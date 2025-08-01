/* Kernel linked list - dynamic nodes */

#define pr_fmt(fmt)    KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/list.h>


// Module parameters
static unsigned int num_nodes = 10; // Default number of nodes
module_param(num_nodes, uint, 0);
MODULE_PARM_DESC(num_nodes, "Number of nodes in the linked list");

static int start_value = 0; // Default start value
module_param(start_value, int, 0);
MODULE_PARM_DESC(start_value, "Start value for the linked list nodes");

static int step_size = 2; // Default step size
module_param(step_size, int, 0);
MODULE_PARM_DESC(step_size, "Step size for the linked list nodes");

// Linked list node definition
struct my_node
{
    int data;
    struct list_head my_list;
};

// Create list head pointer/node
LIST_HEAD(my_head);

static int __init my_mod_init(void)
{
    int count;
    struct my_node *node, *tmp;
    int ret = 0; // Default return value for success

    pr_info("Hello world from %s!\n", KBUILD_MODNAME);
    
    // Create nodes dynamically
    for (count = 0; count < num_nodes; count++)
    {
        // kmalloc node with GFP_KERNEL, which allows sleeping and reclaiming memory
        node = (struct my_node *)kmalloc(sizeof(struct my_node), GFP_KERNEL);
        if (!node) 
        {
            // Log the error using pr_err for kernel messages
            pr_err("Failed to allocate memory for node %d\n", count);
            return 0;  

            // --- CRITICAL CLEANUP ---
            // If memory allocation fails, we must free any nodes that were
            // successfully allocated before the failure to prevent memory leaks.
            pr_info("Cleaning up partially created list...\n");
            list_for_each_entry_safe(node, tmp, &my_head, my_list)
            {
                pr_info("Freeing node with data=%d during error cleanup\n", node->data);
                list_del(&node->my_list); // Remove from list
                kfree(node);              // Free memory
            }
            // --- END CRITICAL CLEANUP ---

            return ret; // Return the error code
        }

        // Set data for the new node
        // Node 0: data = 0 + (0 * 2) = 0
        // Node 1: data = 0 + (1 * 2) = 2
        // Node 2: data = 0 + (2 * 2) = 4
        // ... and so on, matching the user's requirement.
        node->data = start_value + (count * step_size); 

        // Initialize the list_head member of the new node
        INIT_LIST_HEAD(&node->my_list);

        // Add the new node to the linked list (at the head by default)
        list_add(&node->my_list, &my_head);
    }
    
    // Forward traversal to print the linked list contents
    pr_info("Linked list forwards:\n");
    count  = 0; // Reset count for printing
    list_for_each_entry(tmp, &my_head, my_list)
    {
        pr_info("Node %d, data=%d\n", count++, tmp->data);
    }

    return ret; // Return 0 on success, or -ENOMEM if allocation failed
}

static void __exit my_mod_exit(void)
{
    struct my_node *node, *tmp;

    // Iterate safely and delete all nodes, freeing their memory
    list_for_each_entry_safe(node, tmp, &my_head, my_list)
    {
        pr_info("Deleting node with data=%d\n", node->data);
        list_del(&node->my_list); // Remove node from the list
        kfree(node);              // Free the memory allocated for the node
    }
    pr_info("Goodbye world from %s!\n", KBUILD_MODNAME);

    return;
}

// Register module init and exit functions
module_init(my_mod_init);
module_exit(my_mod_exit);

// Module metadata
MODULE_LICENSE("GPL");
MODULE_AUTHOR("EDD <edd@cdac.gov.in>");
MODULE_DESCRIPTION("Kernel linked list - dynamic nodes!");

