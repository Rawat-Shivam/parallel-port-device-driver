#include"header.h"
#include"decleration.h"
#include"datatype.h"

	// Enlist all available operation possible on our device 

void* MainMenu(void* Ret)
{
	int choice;
	printf("%s Begins \n",__FILE__);
	choice = -1;

	printf("press\n\t ._____%s_____\n\n",__func__);

	if(!Ret)
		printf("\t 1. __open device__\n");
	else
	{
		printf("\t 2. __release device__\n");
		printf("\t 3. __write Data__\n");
	}
	printf("\t 0. __exit__\n\nEnter your choice: \n");
	scanf("%d",&choice);

	if(choice == 0)
		(*fptr[choice + 1 ])((void*)Ret);		// calling exitFunc()
	else if((choice == 1)&&(!Ret))
		Ret = (*fptr[choice + 1 ])((void*)Ret);		// calling openFunc()
	else if((choice >= 2)&&(choice <= 3)&&(Ret))
		Ret = (*fptr[choice + 1 ])((void*)Ret);		// calling releaseFunc(),writeData()
	else
		printf("%s Invalid choice \n",__func__);

	printf("%s Ends \n",__FILE__);
	return (void*)Ret;
}
