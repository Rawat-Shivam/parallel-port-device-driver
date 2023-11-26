#include"header.h"
#include"decleration.h"
#include"datatype.h"

	// stops execution of our program which is interacting with our device 

void* exitFunc(void* Ret)
{
	printf("%s Begins \n",__FILE__);
	exit(EXIT_SUCCESS);
	printf("%s Ends \n",__FILE__);
	return 0;
}
