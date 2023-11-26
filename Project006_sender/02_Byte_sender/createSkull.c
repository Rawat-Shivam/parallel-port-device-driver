#include"header.h"
#include"decleration.h"

	/* Since we aren't working on a real device so we are allocating a region in our RAM 
         * to simulate the device .......with help of current function......
         *
         * which first create space for Qset structure then array of pointer then for registers
         * each Qset structure/item will hold 8 register in it.. and size of a register is 4 bytes
	 *
	 * using memset() to remove garbage from device simulation on RAM*/

struct Qset* createSkull(size_t fsize)
{
	int i,j,noi,nor;
	struct Qset *first,*last;
	#ifdef DEBUG
	printk(KERN_INFO "%s Begins\n",__func__);
	#endif

	first = last = NULL;

	// calculating num of item/node required to represent fsize bytes
	noi = ((long)fsize) /(noofReg * Regsize);
	if(((long)fsize) %(noofReg*Regsize))
		noi++;

	for(i=0;i<noi;i++)
	{
		if(!first)
		{
			// allocates memory to starting node 
			first = last = kmalloc(sizeof(struct Qset),GFP_KERNEL);
			if(!last)
			{
				#ifdef DEBUG
				printk(KERN_ERR "%s Error: Qset 1st memory item creation fails\n",__func__);
				#endif

				goto OUT;
			}
			memset(first,'\0',sizeof(Qset));
		}

		else
		{
			// allocates memory to all nodes/item other than starting node
			last->next = kmalloc(sizeof(struct Qset),GFP_KERNEL);
			if(!last->next)
			{
				#ifdef DEBUG
				printk(KERN_ERR "%s Error: Qset memory creation fails\n",__func__);
				#endif
				goto OUT;
			}
			last = last->next;
			memset(last,'\0',sizeof(Qset));
		}
	}

	last = first;
	for(i=0;i<noi;i++)
	{
		// allocates memory to array of pointer within each item 
		last->data = kmalloc(sizeof(char*)*noofReg,GFP_KERNEL);
		if(!last->data)
		{
			#ifdef DEBUG
			printk(KERN_ERR "%s Error: Qset data* memory fails\n",__func__);
			#endif
			goto OUT;
		}
		memset(last->data,'\0',sizeof(char*)*noofReg);
		last = last->next;
	}

	last = first;
	// calculating num of register required to represent fsize bytes when each register can hold 4 byte
	nor = ((long)fsize) / Regsize;
	if(((long)fsize) % Regsize)
		nor++;
	j = 0;
	for(i = 0;i<nor;i++)
	{
		// allocates 4 byte of memory to each register 
		last->data[j] = kmalloc(sizeof(char)*Regsize,GFP_KERNEL);
		if(!last->data[j])
		{
			#ifdef DEBUG
			printk(KERN_ERR "%s Error: Qset data** memory creation fails\n",__func__);
			#endif
			goto OUT;
		}

		memset(last->data[j],'\0',Regsize);

		if(j == (noofReg -1))	// if register count withing current item reaches 7 then we need to move to next item
		{
			last = last->next;
			j = 0;
		}
		else
			j++;	//incrementing register count
	}

	#ifdef DEBUG
	printk(KERN_INFO "%s Ends\n",__func__);
	#endif
	return first;

OUT:
	#ifdef DEBUG
	printk(KERN_ERR "%s Ends (Error) \n",__func__);
	#endif
	return 0;
}
