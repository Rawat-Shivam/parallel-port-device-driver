#include"header.h"
#include"decleration.h"

	/*  In compatiblity mode(...simplex comm...) we are connecting data register of one machine with other machine and using 'nstrobe - busy' pin duo
         * as valid data signal and 'nACK - nstrobe' pin duo as acknowledging path after reception of data
         *
         * But Here(...half-duplex comm...) LSB 4 bit of data register is connected to status register pin(s3,s4,s5,s6) i.e. pin 15,13,12,10 of parallel port
         * and acknowledge path changes to 'busy - nstrobe' pin ....
         * where valid signal duoremain same as that of compatiblity to implement NIBBLE mode in parallel port
         *
         * so Instead of receiving whole byte all in one go as in compatiblity we are receiving original data in patches of
         * 4 - 4 bit and following compatiblity mode for each patch from sender side ..... just instead of reading form data register
         * we are reading data from status register(s3,s4,s5,s6) pin.....
         *
         * finally combining LSB + MSB bit to get original data .... which is written to user space by copy_to_user()
         *
	 * In 05_nibble communication between sender and receiver is simplex(i.e... unidirectional)
	 * Now we are trying to make nibble mode communication half-duplex
	 */

ssize_t writeDev(struct file *filep, const char __user *user, size_t size, loff_t *loff)
{
	int j,noctw,nocsw,nocnw;
	unsigned int temp;
	unsigned char chNib;
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
	
		// displaying 'nstrobe pin' of control register default value
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
		
		// .................. kernel-IO : begins.....................
		// copying least significant 4 bit in chNib
		chNib = **((char**)(item->data+j));
                chNib = chNib << 4;
                chNib = chNib >> 4;
		
		// writing LSB 4 bit on parallel port data register using outb()
		outb(chNib ,BASE_ADDR);
		#ifdef DEBUG
                printk(KERN_INFO "%s outb(lower)---->%d\n",__func__,chNib);
		#endif
		
		outb(1, 0x37A);		// Informing receiver about reception of valid data on line via asserting 'nstrobe pin' of control register
		temp = inb(0x37A);
		temp = temp<<31;
		temp = temp>>31;
		printk(KERN_INFO "After : control_nstrobe(c0)----->%d\n", temp);	
		
		// displaying default value of 'nACK pin' of status register
		temp = inb(0x379);
		temp = temp<<24;
		temp = temp>>31;
		printk(KERN_INFO "Before do loop: status_nACK(s7)----->%d\n", temp);

		do
		{	// checking 'nACK pin' of status register for acknowledgment of recently sent data
			temp = inb(0x379);
			temp = temp<<24;
			temp = temp>>31;
			printk(KERN_INFO "Inside do loop: status_busy(s7)----->%d\n", temp);
			ssleep(1);
		}while(temp);	// will break... when receiver deasserts 'nACK pin' of status register as acknowledgment for successful reception of LSB 4 bit
		temp = inb(0x379);
		temp = temp<<24;
		temp = temp>>31;
		printk(KERN_INFO "After do loop: status_busy(s7)----->%d\n", temp);
		

		outb(0, 0x37A);		// resetting control register to its default value after successful transmission/reception of LSB nibble
		temp = inb(0x37A);
		temp = temp<<31;
		temp = temp>>31;
		printk(KERN_INFO "After : inb(c0)----->%d\n", temp);	

		
		
		// copying MSB 4 bit to chNib
		chNib = **((char**)(item->data+j));
                chNib = chNib >> 4;

                outb(chNib ,BASE_ADDR);		// writing MSB 4 bit of data to parallel port data register
                #ifdef DEBUG
                printk(KERN_INFO "%s outb(upper)---->%d\n",__func__,chNib);
                #endif

		
		// ...........remaining portion is same as above LSB 4 bits...........
                outb(1, 0x37A);
                temp = inb(0x37A);
                temp = temp<<31;
                temp = temp>>31;
                printk(KERN_INFO "After : control_nstrobe(c0)----->%d\n", temp);

                temp = inb(0x379);
                temp = temp<<24;
                temp = temp>>31;
                printk(KERN_INFO "Before do loop: status_busy(s7)----->%d\n", temp);

                do
                {
                        temp = inb(0x379);
                        temp = temp<<24;
                        temp = temp>>31;
                        printk(KERN_INFO "Inside do loop: status_busy(s7)----->%d\n", temp);
                        ssleep(1);
                }while(temp);
                temp = inb(0x379);
                temp = temp<<24;
                temp = temp>>31;
                printk(KERN_INFO "After do loop: status_busy(s7)----->%d\n", temp);

                outb(0, 0x37A);
                temp = inb(0x37A);
                temp = temp<<31;
                temp = temp>>31;
                printk(KERN_INFO "After : control_nstrobe(c0)----->%d\n", temp);
		// .................. kernel-IO : Ends.....................

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
