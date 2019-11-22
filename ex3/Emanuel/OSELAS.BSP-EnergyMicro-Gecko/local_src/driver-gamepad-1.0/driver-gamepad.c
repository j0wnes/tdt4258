/*
 * This is a demo Linux kernel module.
 */


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <stdbool.h>
#include "efm32gg.h"

ssize_t template_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t template_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);
int template_open(struct inode *inode, struct file *filp);
int template_release(struct inode *inode, struct file *filp);
irqreturn_t short_probing(int irq, void *dev_id, struct pt_regs *regs);
static int template_fasync(int fd, struct file* filp, int mode);


#define DEVICE_NAME "template"

static uint8_t irq_value;
static dev_t dev_id;
struct cdev template_cdev;
struct class *cl;
struct fasync_struct* async_queue;



// ldd3 chapter 3, p. 53:
struct file_operations template_fops = {
        .owner =    THIS_MODULE,
        // .llseek =   template_llseek, not used
        .read =     template_read,
        .write =    template_write,
        // .ioctl =    template_ioctl, not used
        .open =     template_open,
        .release =  template_release,
        .fasync = template_fasync
};

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */


static int __init template_init(void)
{
    irq_value = 0;
    // ldd3 chapter 3, p. 45f
    // template_minor = 0, template_count = 1
    if (alloc_chrdev_region(&dev_id, 0, 1, DEVICE_NAME))
    {
        printk(KERN_ERR "Error requesting memory access\n");
        return -1;
    }

    // lld3 chapter 9, p. 249
    // requesting memory regions
    // if everything is ok, non-NULL pointer is returned; error --> NULL-pointer is returned
    if (!request_mem_region((unsigned long)GPIO_PC_MODEL, 4, DEVICE_NAME))
    {
        printk(KERN_ERR "Error allocating memory region GPIO_PC_MODEL\n");
        return -1;
    }
    if (!request_mem_region((unsigned long)GPIO_PC_DOUT, 4, DEVICE_NAME))
    {
        printk(KERN_ERR "Error allocating memory region GPIO_PC_OUT\n");
        return -1;
    }
    if (!request_mem_region((unsigned long)GPIO_EXTIPSELL, 4, DEVICE_NAME))
    {
        printk(KERN_ERR "Error allocating memory region GPIO_EXTIPSELL\n");
        return -1;
    }
    if (!request_mem_region((unsigned long)GPIO_EXTIRISE, 4, DEVICE_NAME))
    {
        printk(KERN_ERR "Error allocating memory region GPIO_EXTIRISE\n");
        return -1;
    }
    if (!request_mem_region((unsigned long)GPIO_IEN, 4, DEVICE_NAME))
    {
        printk(KERN_ERR "Error allocating memory region GPIO_IEN\n");
        return -1;
    }
    if (!request_mem_region((unsigned long)GPIO_IFC, 4, DEVICE_NAME))
    {
        printk(KERN_ERR "Error allocating memory region GPIO_IFC\n");
        return -1;
    }
    if (!request_mem_region((unsigned long)GPIO_IF, 4, DEVICE_NAME))
    {
        printk(KERN_ERR "Error allocating memory region GPIO_IFC\n");
        return -1;
    }

    // ldd3, chapter 3, p. 57
    cdev_init(&template_cdev, &template_fops);
    template_cdev.owner = THIS_MODULE;
    template_cdev.ops = &template_fops;
    if(cdev_add (&template_cdev, dev_id, 1))
    {
        printk(KERN_NOTICE "Error adding template\n");
    }

    cl = class_create(THIS_MODULE, DEVICE_NAME);
    device_create(cl, NULL, dev_id, NULL, DEVICE_NAME);

    printk("Hello World, here is your module speaking\n");
    return 0;
}

/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void)
{
    // freeing all the memory that was previously allocated (ldd3 chapter 9)
    release_mem_region((unsigned long)GPIO_PC_MODEL, 4);
    release_mem_region((unsigned long)GPIO_PC_DOUT, 4);
    release_mem_region((unsigned long)GPIO_EXTIPSELL, 4);
    release_mem_region((unsigned long)GPIO_EXTIRISE, 4);
    release_mem_region((unsigned long)GPIO_IEN, 4);
    release_mem_region((unsigned long)GPIO_IFC, 4);
    release_mem_region((unsigned long)GPIO_IF, 4);

    // ldd3 chapter 10, p. 260: freeing interrupt channel again
    free_irq(17, &template_cdev);
    free_irq(18, &template_cdev);

    unregister_chrdev_region(dev_id, 1);

    printk("Short life for a small module...\n");
}

// ldd3 chapter 3, p. 59
int template_open(struct inode *inode, struct file *filp)
{
    // initializing gamepad, comparable to second assignment
    // see compendium p. 26ff
    //*CMU_HFCORECLKEN0 |= *CMU2_HFPERCLKEN0_GPIO;             // enable clock to GPIO
    *GPIO_PC_MODEL = 0x33333333;        // set pins 0-7 as input
    *GPIO_PC_DOUT = 0xFF;               // enable internal pull-up
    *GPIO_EXTIPSELL = 0x22222222;       // set up interrupt
    *GPIO_EXTIRISE = 0xFF;              // ...on rising edge
    // interrupts for even and odd GPIO ports are 17 and 18 (table 5.2 in compendium)
    // requesting interrupt channel (ldd3 chapter 10, p. 259f)
    if(request_irq(17, (irq_handler_t) short_probing, 0, DEVICE_NAME, &template_cdev))
    {
        printk(KERN_ERR "Error in initiating interrupt handler irq17\n");
        return -1;
    }
    if(request_irq(18, (irq_handler_t) short_probing, 0, DEVICE_NAME, &template_cdev))
    {
        printk(KERN_ERR "Error in initiating interrupt handler irq18\n");
        return -1;
    }

    *GPIO_IEN = 0xFF;           // enable interrupt generation
    *GPIO_IFC = 0xFF;           // clearing all possible interrupts

    printk(KERN_INFO "function template_open was called\n");
    return 0;          /* success */
}

// ldd3 chapter 3, p. 59
int template_release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "function template_release was called\n");
    return 0;
}

// ldd3 chapter 3, p. 67
ssize_t template_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
// value from interrupt handler should be copied/made accessable to user
copy_to_user(buf, &irq_value, 1);

return 0;
}

// ldd3 chapter 3, p. 68f
ssize_t template_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
return 0;
}

// ldd3 chapter 10, p. 276f: interrupt handler
irqreturn_t short_probing(int irq, void *dev_id, struct pt_regs *regs)
{
    // see also compendium p. 28
    // store value of interrupt so that it can be read later
    irq_value = *GPIO_IF;
    // clear interrupt
    *GPIO_IFC = 0xFF;
    printk("interrupt %d \n", irq_value);
    // ldd3, chapter 6, p 171:
    if (async_queue) {
        kill_fasync(&async_queue, SIGIO, POLL_IN);
    }
    return IRQ_HANDLED;
}
// ldd3 chapter 6, p. 169f: asynchronous notification
static int template_fasync(int fd, struct file* filp, int mode) {
    return fasync_helper(fd, filp, mode, &async_queue);
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group 23");
