#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h> /* for alloc_chrdev_region(), unregister_chrdev_region() */
#include<linux/slab.h>	/* for kmalloc() */
#include<linux/cdev.h>	/* for cdev_init(),cdev_add(),cdev_del() */
#include<linux/moduleparam.h>	/* for module_param() */
#include<linux/uaccess.h>	// for copy_from_user()
#include<linux/ioport.h>	// for request_region(), release_region()
#include<linux/delay.h>		// for msleep(), ssleep()
