#include"header.h"
#include"decleration.h"
#include"datatype.h"

	/* Here we are sending an open call to our device at user level which is represented 
	 * in virtual file system as MyDev file*/

void* openFunc(void* Ret)
{
	Holder* lRet;
	char Mode[10];
	printf("%s Begins \n",__FILE__);
	
	lRet = (Holder*)malloc(sizeof(Holder)*1);
	if(!lRet)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	memset(Mode,10,'\n');
	printf("%s Enter mode of opening \n",__FILE__);
	scanf("%s",Mode);	// asking user to enter file mode of opening

	if(strncmp(Mode,"O_RDONLY",8) == 0)
		lRet->fd = open("MyDev",O_RDONLY);

	else if(strncmp(Mode,"O_WRONLY",8) == 0)
		lRet->fd = open("MyDev",O_WRONLY);

	else
		printf("%s Invalid Mode \n",__FILE__);


	if(lRet->fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	else
		printf("%s file opened ---->%d \n",__FILE__,lRet->fd);

	printf("%s Ends \n",__FILE__);
	return (void*)lRet;
}
