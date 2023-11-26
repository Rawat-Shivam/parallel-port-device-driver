#include"header.h"
#include"decleration.h"
#include"datatype.h"

	/* sending a read call to MyDev file through which call is passed to intended device */

void* readData(void* Ret)
{
	int ret;
	Holder* lRet;
	unsigned char ch;
	printf("%s Begins \n",__FILE__);

	lRet = (Holder*)Ret;

	ret = read(lRet->fd,&ch,sizeof(char));	// reading a character data from MyDev file
	if(ret == -1)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	else
		printf("%s %d @ %c \n",__FILE__,ret,ch);

	printf("%s Ends \n",__FILE__);
	return (void*)Ret;
}
