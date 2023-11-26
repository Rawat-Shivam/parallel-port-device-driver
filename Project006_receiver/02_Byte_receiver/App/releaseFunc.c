#include"header.h"
#include"decleration.h"
#include"datatype.h"

	  /* Here we are sending close call to our device representataion in vfs as MyDev*/

void* releaseFunc(void* Ret)
{
	Holder* lRet;
	printf("%s Begins \n",__FILE__);
	lRet = (Holder*)Ret;

	close(lRet->fd);
	lRet = NULL;

	printf("%s Ends \n",__FILE__);
	return (void*)lRet;
}
