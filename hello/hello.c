#include <linux/init.h>
#include <linux/kernel.h> 
#include <linux/module.h> 
/*
to run this module
1- make
2- sudo insmod hello.ko
3- sudo -i
- dmesg
4- sudo rmmod hello
*/

MODULE_AUTHOR("Ola Ibrahim");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A simple Hello World!");
MODULE_VERSION("1.0");

/* This function is called when the module is loaded. */
// default is "hellow world" but you can change it by passing parameter
static int __init hello_init(void)
{
    printk(KERN_INFO "Hello, worldxxxxx!\n");
    return 0;
}

/* This function is called when the module is removed. */
static void __exit hello_exit(void)
{
    printk(KERN_INFO "Goodbye, world!\n");
}
// init for init_module its called when insmod is called
module_init(hello_init);
// exit for cleanup_module its called when rmmod is called
module_exit(hello_exit);

MODULE_LICENSE("GPL");
