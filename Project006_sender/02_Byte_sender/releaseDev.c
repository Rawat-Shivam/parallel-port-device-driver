#include"header.h"
#include"decleration.h"

	/* function free up memory space pointed by pointer obtained from container_of() macro
	 * and alongwith it destroies stream created by container_of() macro */

int releaseDev(struct inode* Inode, struct file* File)
{
	#ifdef DEBUG
	printk(KERN_INFO "%s Begins\n",__func__);
	#endif
	#ifdef DEBUG
	printk(KERN_INFO "%s Ends\n",__func__);
	#endif
	return 0;
}
