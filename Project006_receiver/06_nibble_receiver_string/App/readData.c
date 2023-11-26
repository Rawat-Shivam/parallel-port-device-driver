#include"header.h"
#include"decleration.h"
#include"datatype.h"

	/* sending a read call to MyDev file through which call is passed to intended device */

void* readData(void* Ret)
{
	int ret;
	Holder* lRet;
	char str[200];
	printf("%s Begins \n",__FILE__);

	lRet = (Holder*)Ret;
	memset(str,'\0',200);

	ret = read(lRet->fd,str,200);	// reading data from MyDev file
	if(ret == -1)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	else
		printf("%s %d @ %s \n",__FILE__,ret,str);

	printf("%s Ends \n",__FILE__);
	return (void*)Ret;
}
