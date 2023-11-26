#include"header.h"
#include"decleration.h"
#include"datatype.h"

	// aliasing name for user defined function are mapped here with array of function pointer

int init()
{
	printf("%s Begins \n",__FILE__);

	(fptr[0]) = MainMenu; 
	(fptr[1]) = exitFunc;
	(fptr[2]) = openFunc;
	(fptr[3]) = releaseFunc;
	(fptr[4]) = writeData;
	(fptr[5]) = readData;

	printf("%s Ends \n",__FILE__);
	return 0;
}
