#include"header.h"
#include"decleration.h"

	/* In 03_compatiblity_char we are just receiving a character from other machine with acknowledging sender about
         * reception of recently read data..... Here we are receiving a complete string instead of single character from other machine
	 *
	 * Finally copying data from kernel buffer to user space
         */


ssize_t readDev(struct file *filep, char __user *user, size_t size, loff_t *loff)
{
	unsigned char temp;
	char chData,chArr[DEVSIZE];
	int i;
	#ifdef DEBUG
	printk(KERN_INFO "%s Begins\n",__func__);
	#endif

	i = 0;
	memset(chArr,'\0',DEVSIZE);
       
       	do
	{
		// ........... IO-kernelbuffer : Begins .............
		outb(32,0x37A);		// changing direction of data register from output to input by asserting 5th bit of control register 

		// displaying default value of 'busy pin' of status register
		temp = inb(0x379);
        	temp = temp>>7;
		#ifdef DEBUG
        	printk(KERN_INFO "status_busy(s7)---->%d\n",temp);
		#endif
	
		do
        	{	// checking whether 'busy pin' of status register asserts via sender
        	        temp = inb(0x379);
        		temp = temp>>7;
			#ifdef DEBUG
        	        printk(KERN_INFO ".....inside do while status_busy(s7)----->%d......\n",temp);
			#endif
        	        msleep(1000);
        	}while(!temp);	// will break.... when 'busy pin' of status register assert by sender to indicate availability of valid new data on line

        	chData = inb(0x378);	// reading valid new data from parallel port's data register by inb()
		chArr[i++] = chData;
		#ifdef DEBUG
	        printk(KERN_INFO "chData--------->%c\n",chData);
		#endif

	        outb(1,0x37A);	// sending acknowledgment to sender after successful reception of character data via asserting 'nstrobe pin' of receiver
	        temp = inb(0x37A);
	        temp = temp << 7;
	        temp = temp >> 7;
		#ifdef DEBUG
	        printk(KERN_INFO " control_nstrobe(c0)--------->%d\n",temp);
		#endif

	        ssleep(1);
	        outb(0,0x37A);		// resetting control register to its default value after successful transmission/reception of data
	        temp = inb(0x37A);
	        temp = temp << 7;
	        temp = temp >> 7;
		#ifdef DEBUG
	        printk(KERN_INFO "Control strobe in end--------->%d\n",temp);
		#endif
		// ........... IO-kernelbuffer : Ends .............
	}while(chData != '\0');



	#ifdef DEBUG
        printk(KERN_INFO "chArr--------->%s\n",chArr);
	#endif
	
	// copying data from kernel buffer to user space using copy_to_user()
	if( copy_to_user(user, chArr, i-1 ) != 0)
	{
		#ifdef DEBUG
		printk(KERN_INFO "Partial read\n");
		#endif
	}

	#ifdef DEBUG
	printk(KERN_INFO "%s Ends\n",__func__);
	#endif
	return i - 1;
}
