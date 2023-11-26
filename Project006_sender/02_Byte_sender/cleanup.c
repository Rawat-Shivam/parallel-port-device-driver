#include"header.h"
#include"decleration.h"

/* deregistering space allocated for device using unregister_chrdev_region() */

static void __exit cleanup(void)
{
	int i;

	// releasing port address acquired by request_region()
	release_region(BASE_ADDR, NUM);

	for(i=0;i<nodev;i++)
		/* freeing up entry of cdev structure for each device */
		cdev_del(&dev[i].c_dev);
	kfree(dev);	/* deallocating space allocated by kmalloc */
	unregister_chrdev_region(deviceId,nodev);	/* unregistering device from device table */
	
	#ifdef DEBUG
	printk(KERN_INFO "Thank you for device table entry !!!\n");
	#endif
}
module_exit(cleanup);
