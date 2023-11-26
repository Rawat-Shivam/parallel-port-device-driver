#include"header.h"
#include"decleration.h"

	/* If device is opened in write mode then we need to erase off previous data present in device
	 * before allowing user to write new data as to avoid errors....
	 *
	 * first free up space occupied by registes then array of pointer then item itself */

int trimDev(struct Dev *fdev)
{
	int i;
	struct Qset *item,*slitem;
	#ifdef DEBUG
	printk(KERN_INFO "%s Begins\n",__func__);
	#endif

	if(!fdev)	// checks whether pointer passed as argument to trimDev() is NULL or not
	{
		#ifdef DEBUG
		printk(KERN_ERR "%s Error: passed dev structure pointer is NULL\n",__func__);
		#endif
		goto OUT;
	}
	else if(!fdev->first)	// if device is opened in writemode first time then return from here only with 0 value
	{
		#ifdef DEBUG
		printk(KERN_INFO "%s No data present on device\n",__func__);
		#endif
		goto RET;
	}
	else
	{
		#ifdef DEBUG
		printk(KERN_INFO "%s before: fdev....while()\n",__func__);
		#endif
		while(fdev->first)	// if device is opened in writemode more than one time in lifespan then execute following lines
		{
			#ifdef DEBUG
			printk(KERN_INFO "%s Before: item....while()\n",__func__);
			#endif
			item = slitem = fdev->first;	
			while(item->next)
			{// following while loop determine last , 2ndlast item of device
				slitem = item;
				item = item->next;
			}
	
			#ifdef DEBUG
			printk(KERN_INFO "%s before: item->data[i]....for()\n",__func__);
			#endif
			for(i=fdev->noofReg-1; i >= 0; i--)
			{
				// if ith register exists then free up space occupied by it 
				if(item->data[i])
				{
					kfree(item->data[i]);
					item->data[i] = NULL;
				}
			}

			kfree(item->data);	// free up space occupied by array of pointers within a node to hold register address
			item->data = NULL;	// assgin NULL to avoid dangling pointer
			
			if(item == slitem)	
			{
				kfree(fdev->first);
				// execute only when there is only one item present in device then last,slitem pointer points to same location
				fdev->first = NULL;
			}
			else
			{	
				kfree(slitem->next);	// free up space occupied by node/item
				slitem->next = NULL;
			}
		}
		
		fdev->datasize = datasize = 0;	// update datasize global and Dev datamember to 0 in order to avoid segmentation fault
	}

RET:
	#ifdef DEBUG
	printk(KERN_INFO "%s Ends\n",__func__);
	#endif
	return 0;

OUT: 
	#ifdef DEBUG
	printk(KERN_ERR "%s Ends(ERROR)\n",__func__);
	#endif
	return -1;
}
