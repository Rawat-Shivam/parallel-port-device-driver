#include"header.h"
#include"decleration.h"

	/* In 02_Byte_receiver we are just receiving a character from other machine without acknowledging sender about
         * reception of recently read data..... so Here we are using 'nstrobe(of receiver) - nACK(of sender)' pin duo
         * to form acknowledging path after reception of data......
         *
         * i.e.sender continuosly checking 'nACK pin' of status register which is deasserts by receiver via
         * asserting 'nstrobe pin' of control register after successful reception of recently sent data
	 *
	 * Finally copying data from kernel buffer to user space
         */


ssize_t readDev(struct file *filep, char __user *user, size_t size, loff_t *loff)
{
	unsigned char temp;
	char chData;
	#ifdef DEBUG
	printk(KERN_INFO "%s Begins\n",__func__);
	#endif
       
       	
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



	// copying data from kernel buffer to user space using copy_to_user()
	if( copy_to_user(user, &chData, sizeof(char)) != 0)
	{
		#ifdef DEBUG
		printk(KERN_INFO "Partial read\n");
		#endif
	}

	#ifdef DEBUG
	printk(KERN_INFO "%s Ends\n",__func__);
	#endif
	return 1;
}
