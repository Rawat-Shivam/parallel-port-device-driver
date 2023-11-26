#include"header.h"
#include"decleration.h"
#include"datatype.h"

	/* Calling MainMenu() in an infinite loop which enlist available operation
	 * possible on our device */

int main()
{
	Holder *Ret;
	Ret = NULL;

	init();	// calling init() 

	printf("%s Begins \n",__FILE__);

	while(1)
	{
	       	Ret = (Holder*)(*fptr[0])((void*)Ret);	// calling mainMenu() 
	}

	printf("%s Ends \n",__FILE__);
	return 0;
}
