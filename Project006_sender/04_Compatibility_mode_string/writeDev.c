#include"header.h"
#include"decleration.h"

	/* Initially creating device simulation in RAM with help of createSkull() then writing 4 bytes at a time on a device
	 * until lsize is greater than zero using a copy_from_user() macro 
	 *
	 * In 03_compatiblity_char we are just transmitting a character data between 2 machine with acknowledging 
	 * reception of recently sent data..... Here we are transmitting a string b/w 2 machine instead of single character only
	 */

ssize_t writeDev(struct file *filep, const char __user *user, size_t size, loff_t *loff)
{
	int j,noctw,nocsw,nocnw;
	unsigned int temp;
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
	
		// displaying default value of 'nstrobe pin' of control register
	       	temp = inb(0x37A);
		temp = temp<<31;
		temp = temp>>31;
		printk(KERN_INFO "Before : control_nstrobe(c0)----->%d\n", temp);	
		/* copying noctw(num of character to write) bytes from user buffer to jth register present in 
		 * current item/node with help of copy_from_user() macro.....which take 3 argument....
		 * i.e. ... destination address,source address, num of bytes to write
		 * and returns num of character not written in destination 
		 */
		nocnw = copy_from_user(*(item->data + j),(user + nocsw) ,noctw);
		if(nocnw != 0)
		{
			#ifdef DEBUG
			printk(KERN_ERR "%s Partial Write\n",__func__);
			#endif
		}
	


		// ........ data copy from kernel-> IO register : Begins............
		/* writing data to IO ports using outb() .... take 2 argument i.e. 
		 * data value to be passed, IO address of register
		 */
		outb(**((char**)(item->data+j)),BASE_ADDR);
		#ifdef DEBUG
		printk(KERN_INFO "%s outb---->%c\n",__func__,**((char**)(item->data + 0)));
		#endif
		
		// setting 'nstrobe pin' of control register to make receiver aware of new valid data on line
		outb(1, 0x37A);
		temp = inb(0x37A);
		temp = temp<<31;
		temp = temp>>31;
		printk(KERN_INFO "After : control_nstrobe(c0)----->%d\n", temp);	
		
		// displaying default value of 'nACK pin' of status register
		temp = inb(0x379);
		temp = temp<<25;
		temp = temp>>31;
		printk(KERN_INFO "Before do loop: status_nACK(s6)----->%d\n", temp);

		do
		{
			// waiting for receiver acknowledgment for recently sent data byte
			temp = inb(0x379);
			temp = temp<<25;
			temp = temp>>31;
			printk(KERN_INFO "Inside do loop: status_nACK(s6)----->%d\n", temp);
			ssleep(1);
		}while(temp);	// will break.... when 'nACK pin' of status register is deasserts by receiver after reading recently sent data byte

		// displaying 'nACK pin' after being reset by receiver
		temp = inb(0x379);
		temp = temp<<25;
		temp = temp>>31;
		printk(KERN_INFO "After do loop: status_nACK(s6)----->%d\n", temp);
		
		// resetting 'nstrobe pin' of control register after transmission/reception of character data b/w 2 machine
		outb(0, 0x37A);
		temp = inb(0x37A);
		temp = temp<<31;
		temp = temp>>31;
		printk(KERN_INFO "After : control_nstrobe(c0)----->%d\n", temp);	
		// ........ data copy from kernel-> IO register : Ends............



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
