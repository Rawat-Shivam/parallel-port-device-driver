#include"header.h"
#include"decleration.h"

	/* Initially creating device simulation in RAM with help of createSkull() then writing 4 bytes at a time on a device
	 * until lsize is greater than zero using a copy_from_user() macro 
	 *
	 * We are just copying data from user space to kernel buffer then transmitting data to user space as when requested by reader
	 * so uptil now we haven't write anything on our real device.....because its not merely available.....
	 *
	 * First time in character driver we are communicating with our real device using IO ports(or here ... parallel port)
	 * by transmitting a ascii character from kernel space to IO ports using outb()
	 */

ssize_t writeDev(struct file *filep, const char __user *user, size_t size, loff_t *loff)
{
	int j,noctw,nocsw,nocnw;
	size_t lsize;
	Dev* ldev;
	struct Qset *item;
	#ifdef DEBUG
	printk(KERN_INFO "%s Begins\n",__func__);
	#endif
	
	// extracting Dev structure pointer from private_data field of struct file
	ldev = (Dev*)filep->private_data;
	if(!ldev)
	{
		#ifdef DEBUG
		printk(KERN_ERR "%s....Error: ldev is null.... \n",__func__);
		#endif
		goto OUT;
	}
	
	// calculating size of data to be written of device
	if( size > ldev->devsize )
		lsize = ldev->devsize;
	else
		lsize = size;

	// calling createSkull to simulate device in our RAM 
	ldev->first = createSkull(lsize);
	if(!ldev->first)
	{
		#ifdef DEBUG
		printk(KERN_ERR "%s createSkull() fails\n",__func__);
		#endif
		goto OUT;
	}

	nocnw = noctw = nocsw = j = 0;
	item = ldev->first;

	while(lsize > 0)	// following while loops runs until lsize > zero
	{
		/* determining num of bytes written on one register ....if lsize greater than Regsize
		 * then we will write only 4 bytes in current iteration else we write lsize bytes*/
		if(lsize > ldev->Regsize)
			noctw = ldev->Regsize;
		else
			noctw = lsize;
		
		/* copying noctw(num of character to write) bytes from user buffer to jth register present in 
		 * current item/node with help of copy_from_user() macro.....which take 3 argument....
		 * i.e. ... destination address,source address, num of bytes to write
		 * and returns num of character not written in destination 
		 * */
		nocnw = copy_from_user(*(item->data + j),(user + nocsw) ,noctw);
		if(nocnw != 0)
		{
			#ifdef DEBUG
			printk(KERN_ERR "%s Partial Write\n",__func__);
			#endif
		}


		/* writing data to IO ports from kernel space using outb().... takes 2 argument 
		 * i.e. value , IO address of register	
		 */
		outb(**((char**)(item->data+j)),BASE_ADDR);
		#ifdef DEBUG
		printk(KERN_INFO "%s outb---->%c\n",__func__,**((char**)(item->data + 0)));
		#endif


		lsize -= (noctw - nocnw);	// modifying lsize 
		nocsw += (noctw - nocnw);	// updating nocsw(num of character successfully written) variable 
		*(int*)loff += (noctw - nocnw); // updating file pos of loff_t type local variable
		datasize = nocsw;

		if(j == (ldev->noofReg-1))	// if register count in current item reaches 7th then we need to move to next item
		{	
			j = 0;
			item = item->next;
		}
		else 
			j++;			// incrementing register count
	}
	
	filep->f_pos = nocsw;		// updating file pos of struct file
	ldev->datasize = datasize;		// storing nocsw varaible value in datasize field of Dev structure
	
	#ifdef DEBUG
	printk(KERN_INFO "%s ldev->datasize---->%d\n",__func__,ldev->datasize);
	#endif
	
	#ifdef DEBUG
	printk(KERN_INFO "%s Ends\n",__func__);
	#endif
	return nocsw;

OUT:
	#ifdef DEBUG
	printk(KERN_ERR "%s Ends(Error)\n",__func__);
	#endif
	return -1;
}
