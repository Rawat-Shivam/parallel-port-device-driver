#include"header.h"
#include"decleration.h"
#include"datatype.h"

	/* writing data to MyDev special file through which data is written on intended device */

void* writeData(void* Ret)
{
	int ret;
	Holder* lRet;
	unsigned char ch;
	lRet = (Holder*)Ret;

	printf("%s Begins----->%d \n",__FILE__,lRet->fd);

	printf("%s Enter data \n",__FILE__);	
	scanf(" %c",&ch);

	ret = write(lRet->fd,&ch,1);	// writing one character data on MyDev file
	if(ret == -1)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}
	else
		printf("%s %d @ %c \n",__FILE__,ret,ch);

	printf("%s Ends \n",__FILE__);
	return (void*)Ret;
}
