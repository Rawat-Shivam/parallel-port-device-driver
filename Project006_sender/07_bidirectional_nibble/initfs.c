#include"header.h"
#include"decleration.h"
#include"fileOps.h"

	/* Initially registering our device to kernel in device table by alloc_chrdev_region() ...then making kernel side entry of 
	 * device by adding cdev type structure to device table via cdev_add() which is been initialised by cdev_init()
	 */

/* extern variable decleration */
int majorno,minorno,nodev,devsize,datasize,noofReg,Regsize;
dev_t deviceId;
Dev *dev;

module_param(nodev,int,S_IRUGO);	/* accepting value of 'nodev' variable at run time using module_param() */
module_param(noofReg,int,S_IRUGO);	/* accepting value of 'noofReg' variable at run time using module_param() */
module_param(Regsize,int,S_IRUGO);	/* accepting value of 'Regsize' variable at run time using module_param() */

static int __init helloK(void)
{
	dev_t local_deviceId;
	int i;
	majorno = MAJORNO;
	minorno = MINORNO;
	devsize = DEVSIZE;
	datasize = DATASIZE;

	#ifdef DEBUG
	printk(KERN_INFO "Hello kernel !!!\n");
	printk(KERN_INFO "No of devices---->%d \n",nodev);
	#endif

	/* we are making an entry for our device in device table using alloc_chrdev_region() which takes 4 argument i..e..
	 * a pointer of dev_t to store devie id,
	 * a unsigned minornor , num of consecutive devices
	 * and device name ....
	 * 
	 * return 0 on success and -1 on failure */
	if(alloc_chrdev_region(&deviceId,minorno,nodev,DEVNAME) < 0)
	{
		printk(KERN_ALERT "alloc_chrdev_region() fails\n");
		goto OUT;
	}
	else
	{
		majorno = MAJOR(deviceId);	/* extracting major num from deviceId using macro */
		printk(KERN_INFO "major no is--->%d  !!!\n",majorno);

		/* allocating space for our device in kernel space using kmalloc() which take 2 arguments i..e..
		 * size of memory to be allocated ,
		 * and flag for exeption case behaviour when allocating memory to our device 
		 */
		dev = (Dev*)kmalloc(sizeof(Dev)*nodev,GFP_KERNEL);
		if(!dev)
		{
			printk(KERN_ALERT "kmalloc() fails\n");
			goto OUT;
		}
		printk(KERN_INFO "Memory to dev is allocated successfully\n");

		for(i=0;i<nodev;i++)
		{
			 memset(dev+i,'\0',sizeof(Dev));
			/* Initialising a cdev structure type object with default value according to our program
			 * via cdev_init() which take 2 argument i.e....a pointer of struct cdev type 
			 * and a pointer of struct file_operations type.......
			 *
			 * whose link is been added in device table later on by cdev_add()
			 * which take 3 arguments i.e.....
			 * pointer to cdev structure object initialise by cdev_init()
			 * ,deviceId for the which this link is created
			 * and no of devices in consecutive for which this structure link is valide ...usually one
			 *
			 * returns zero on success and -1 on failure
			 */
			cdev_init(&dev[i].c_dev, &fops);

			dev[i].devsize = devsize;
			dev[i].datasize = datasize;
			dev[i].noofReg = noofReg;
			dev[i].Regsize = Regsize;

			local_deviceId = MKDEV(majorno,(i + MINORNO));
			
			/* making our device hardware entry in device table using cdev_add() */
			if(cdev_add(&dev[i].c_dev,local_deviceId,nodev) < 0)
			{
				printk(KERN_ALERT "cdev_add() fails\n");
				goto OUT;
			}
			printk(KERN_INFO "major no from c_dev structure is--->%d  !!!\n",MAJOR(dev[i].c_dev.dev));
			minorno = MINOR(dev[i].c_dev.dev);
			printk(KERN_INFO "minor no is--->%d  !!!\n",minorno);
		}

RET:
		/* requesting for exclusive access of IOports via request_region().... which take 3 argument i.e....
		 * base address of port, num of consecutive address after base, name to acquired port
		 * through which data to be written/read  on/from device
		 *
		 * ...... returns NULL on failure ......
		 */
		if( !request_region(BASE_ADDR, NUM, PARALLEL_NAME) )
		{
			/* if partial num of address are acquire via request_region() then we need to free
			 * those partial acquired address via release_region() then make a fresh attempt 
			 * to acquire all the requested address .....
			 *
			 * this cycle must be repeated until unless all requested address are acquired
			 */
			release_region(BASE_ADDR, NUM);
			printk(KERN_INFO "Error: Unable to  acquire Port  !!!\n");
			goto RET;	// moving flow control to RET , In order to make fresh attempt for port's exclusive access
		}
		else
		{
			printk(KERN_INFO ".......Port acquired successfully  !!!.......\n");
			outb(1,BASE_ADDR);
			ssleep(2);
			outb(2,BASE_ADDR);
			ssleep(2);
			outb(4,BASE_ADDR);
			ssleep(2);
			outb(8,BASE_ADDR);
			ssleep(2);
			outb(16,BASE_ADDR);
			ssleep(2);
			outb(255,BASE_ADDR);
			ssleep(2);
}

		return 0;
	}
OUT:
	return -1;
}

module_init(helloK);
