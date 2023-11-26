#include"header.h"
#include"decleration.h"

	/* Initially creating device simulation in RAM with help of createSkull() then writing 4 bytes at a time on a device
	 * until lsize is greater than zero using a copy_from_user() macro 
	 *
	 * Transmitting data between 2 machine using parallel port BYTE mode 
	 * where 'nstrobe pin' of sender's control register is connected to 'busy pin' of receiver's status register
	 * No acknowledgment is been sent by receiver after reading data
	 *
	 * working ---- 
	 *  ~ sender copy data from kernel buffer to IO data register of parallel port via outb()
	 *  ~ then it asserts 'nstrobe pin' of control register to make receiver aware of valid new data on line
	 *  ~ thereby receiver reads data as soon as 'busy pin' of status register is changed
	 *  ~ after random delay sender deasserts nstrobe pin ....
	 */

ssize_t writeDev(struct file *filep, const char __user *user, size_t size, loff_t *loff)
{
	int j,noctw,nocsw,nocnw;
	unsigned int temp;	// for storing return value of inb()
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
	


		//............ Kernel buffer to IO port : Begins.................
		/* Transfering data from kernel buffer to IO ports using outb().... take 2 argument i.e.
		 * data to passed, IO address of register
		 */
		outb(**((char**)(item->data+j)),BASE_ADDR);
		#ifdef DEBUG
		printk(KERN_INFO "%s outb---->%c\n",__func__,**((char**)(item->data + 0)));
		#endif

		/* ..... setting nstrobe pin of control register  .... 
		 * In order to make receiver aware that valid new data is available on data line
		 */
                outb(1, 0x37A);
		// displaying nstrobe pin of control register after setting		
                temp = inb(0x37A);
                temp = temp<<31;
                temp = temp>>31;
		#ifdef DEBUG
                printk(KERN_INFO "Setting: control_nstrobe (c0)----->%d\n", temp);
		#endif

                ssleep(10);	// delaying resetting of nstrobe pin of control register.... so that receiver have time to read data 
                outb(0, 0x37A);	// resetting nstrobe pin
		// displaying nstrobe pin of control reigster after resetting
                temp = inb(0x37A);
                temp = temp<<31;
                temp = temp>>31;
		#ifdef DEBUG
                printk(KERN_INFO "Resetting: control_nstrobe (c0)----->%d\n", temp);
		#endif
		//............ Kernel buffer to IO port : Ends.................


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
