#include"header.h"
#include"decleration.h"

	/* Transmitting data between 2 machine using parallel port BYTE mode 
         * where 'nstrobe pin' of sender's control register is connected to 'busy pin' of receiver's status register
         * No acknowledgment is been sent by receiver afte reading data
         *
         * working ---- 
         *  ~ sender copy data from kernel buffer to IO data register of parallel port via outb()
         *  ~ then it asserts 'nstrobe pin' of control register to make receiver aware of valid new data on line
         *  ~ thereby receiver reads data as soon as 'busy pin' of status register is changed
         *  ~ after random delay sender deasserts nstrobe pin ....
	 *
	 *  Finally read data is passed to user space using copy_to_user()
	 */

ssize_t readDev(struct file *filep, char __user *user, size_t size, loff_t *loff)
{
	unsigned char temp;
	char chData;
	#ifdef DEBUG
	printk(KERN_INFO "%s Begins\n",__func__);
	#endif

	/* changing direction of information flow of parallel port data register
	 * from output to input by asserting 5th bit of control register 
	 */
	outb(32,0x37A);
	
	// displaying 'busy pin' of status register before being set by sender's nstrobe pin of control register
	temp = inb(0x379);
	temp = temp>>7;
	printk(KERN_INFO "Before: status_busy(s7)---->%d\n",temp);
		
	do
	{
		// checks whether busy pin of status register set by sender's nstrobe pin of control register
		temp = inb(0x379);
		temp = temp>>7;
		#ifdef DEBUG
		printk(KERN_INFO ".....inside while status_busy(s7)----->%d......\n",temp);
		#endif
		msleep(1000);
	}while(!temp);	// breaks when.... busy pin of receiver's status register set by nstrobe pin of sender's control register

	/* reading data from parallel port data register using inb() ... which take IO register address as argument
	 *
	 * ...... returns 32 bit value present in IO register ....... */
	chData = inb(0x378);	
	#ifdef DEBUG
	printk(KERN_INFO "chData--------->%c\n",chData);
	#endif

	// copying data from kernel space to user buffer via copy_to_user()
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
