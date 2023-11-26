#include"header.h"
#include"decleration.h"

	/* We are allowing our device to open up by user app once all the checks have turned out to be true
	 * and also extracting memory pointer to Dev user-defined structure and storing it in private data field of struct file structure */

int openDev(struct inode* Inodp, struct file* Filep)
{
	Dev *ldev;
	#ifdef DEBUG
	printk(KERN_INFO "%s Begins\n",__func__);
	#endif

	/* extracting a pointer of Dev structure with help of container_of() macro.....which take 3 argument i.e.
	 * device pointer stored in struct inode structure,
	 * user-defined structure for device,
	 * and kernel representation of device.
	 * 
	 * returns a pointer which can point in kernel space as well as I/O space 
	 * + creates stream i.e. struct file pointer initialization
	 */
	ldev = (Dev*)container_of(Inodp->i_cdev,Dev,c_dev);	// getting a Dev structure pointer 
	if(!ldev)
	{
		#ifdef DEBUG
		printk(KERN_ERR "%s Error: container_of() macro fails\n",__func__);
		#endif

		goto OUT;
	}

	/* checking whether mode of opening passed by user for the device is writemode or not
	 * if yes , then call trimDev() to erase previous data */
	if(( O_ACCMODE & Filep->f_flags) == O_WRONLY)
		// execute only when user have asked to open device in writemode
		trimDev((void*)ldev);
	
	Filep->private_data = (void*)ldev;	// storing Dev structure pointer in private_data data member of struct file structure
	#ifdef DEBUG
	printk(KERN_INFO "%s Ends\n",__func__);
	#endif
	return 0;

OUT:
	#ifdef DEBUG
	printk(KERN_ERR "%s Ends(Error)\n",__func__);
	#endif
	return -1;
}
