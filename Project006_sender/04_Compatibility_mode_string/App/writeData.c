#include"header.h"
#include"decleration.h"
#include"datatype.h"

	/* writing data to MyDev special file through which data is written on intended device */

void* writeData(void* Ret)
{
	int ret;
	Holder* lRet;
	char str[] = "Emblogic research competency and development labs.....";
	lRet = (Holder*)Ret;
	printf("%s Begins----->%d \n",__FILE__,lRet->fd);

	ret = write(lRet->fd,str,sizeof(str));	// writing data on MyDev file
	if(ret == -1)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}
	else
		printf("%s %d @ %s \n",__FILE__,ret,str);

	printf("%s Ends \n",__FILE__);
	return (void*)Ret;
}
