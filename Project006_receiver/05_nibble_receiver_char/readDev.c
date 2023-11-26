#include"header.h"
#include"decleration.h"

	/* In compatiblity mode we are connecting data register of one machine with other machine and using 'nstrobe - busy' pin duo
         * as valid data signal and 'nACK - nstrobe' pin duo as acknowledging path after reception of data
         *
         * But Here LSB 4 bit of data register is connected to status register pin(s3,s4,s5,s6) i.e. pin 15,13,12,10 of parallel port
         * where valid signal duo, acknowledge path duo remain same as that of compatiblity to implement NIBBLE mode in parallel port
         *
         * so Instead of receiving whole byte all in one go as in compatiblity we are receiving original data in patches of 
         * 4 - 4 bit and following compatiblity mode for each patch from sender side ..... just instead of reading form data register
	 * we are reading data from status register(s3,s4,s5,s6) pin.....
	 *
	 * finally combining LSB + MSB bit to get original data .... which is written to user space by copy_to_user()
	 */

ssize_t readDev(struct file *filep, char __user *user, size_t size, loff_t *loff)
{
	unsigned char temp;
	char chData, NibL,NibM;	// nib least ,nib most significant 
	#ifdef DEBUG
	printk(KERN_INFO "%s Begins\n",__func__);
	#endif
        
	// .........IO-Kernel :Begins..............
	// getting LSB 4 bit of original data
	outb(32,0x37A);		// changing direction of data register from output to input via setting 5th bit of control register

	// displaying default value of 'nstrobe pin' of control register
	temp = inb(0x379);
        temp = temp>>7;
	#ifdef DEBUG
        printk(KERN_INFO "status_busy(s7)---->%d\n",temp);
	#endif
	do
        {
                temp = inb(0x379);
        	temp = temp>>7;
		#ifdef DEBUG
                printk(KERN_INFO ".....inside do while status_busy(s7)----->%d......\n",temp);
		#endif
                msleep(1000);
        }while(!temp);

        NibL = inb(0x379);	// fetching 4 LSB bit of original data from status register
	NibL = NibL << 1;
	NibL = NibL >> 4;
	#ifdef DEBUG
        printk(KERN_INFO "NibL(lower)--------->%c\n",NibL);
	#endif

        outb(1,0x37A);		// sending ack signal to sender after reading 4 LSB of original data
        temp = inb(0x37A);
        temp = temp << 7;
        temp = temp >> 7;
	#ifdef DEBUG
        printk(KERN_INFO "control_nstrobe(c0)--------->%d\n",temp);
	#endif
        ssleep(1);

        outb(0,0x37A);
        temp = inb(0x37A);
        temp = temp << 7;
        temp = temp >> 7;
	#ifdef DEBUG
        printk(KERN_INFO " control_nstrobe(c0)--------->%d\n",temp);
	#endif

	
	
	// getting MSB 4 bit of original data
	outb(32,0x37A);

        temp = inb(0x379);
        temp = temp>>7;
        #ifdef DEBUG
        printk(KERN_INFO "status_busy(s7)---->%d\n",temp);
        #endif
        do
        {
                temp = inb(0x379);
                temp = temp>>7;
                #ifdef DEBUG
                printk(KERN_INFO ".....inside do while status_busy(s7)----->%d......\n",temp);
                #endif
                msleep(1000);
        }while(!temp);	// will break.... when 'busy pin' of receiver is set by sender to indicate valid new data availability on line by asserting its 'nstrobe pin'

        NibM = inb(0x379);	// getting 4 MSB bit of original data
        NibM = NibM >> 3;
        NibM = NibM << 4;
        #ifdef DEBUG
        printk(KERN_INFO "NibM(upper)--------->%c\n",NibM);
        #endif

	chData = NibL | NibM;	// combining LSB + MSB to get the original data
        #ifdef DEBUG
        printk(KERN_INFO "chData--------->%c\n",chData);
        #endif


        outb(1,0x37A);
        temp = inb(0x37A);
        temp = temp << 7;
        temp = temp >> 7;
        #ifdef DEBUG
        printk(KERN_INFO "control_nstrobe(c0)--------->%d\n",temp);
        #endif
        ssleep(1);

        outb(0,0x37A);		// resetting control register to its default value after successful transmission/reception of a character by NIBBLE mode
        temp = inb(0x37A);
        temp = temp << 7;
        temp = temp >> 7;
        #ifdef DEBUG
        printk(KERN_INFO "control_nstrobe(c0)--------->%d\n",temp);
	#endif
	// .........IO-Kernel :ends..............

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
