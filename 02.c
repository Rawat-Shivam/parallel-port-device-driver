#include<stdlib.h>
#include<stdio.h>

	/* Every character have 2 fields attach with it.... i.e its symbol and ascii
	 * so when using %c to get input for a char variable -------- then asking user to provide symbol
	 * i.e. if user enter 7 then symbol---'7' ascii----55
	 *
	 * but when using %d to get input for a char variable ----- then asking user to provide ascii of symbol
	 * i.e. if user enter 7 thne symbol---'\a' ascii----7
	 *
	 * Note--- this difference exists only when numeric data is enter.... it is hard to observe in character data
	 */

int main()
{
	char ch;
	printf("%s Begins\n",__func__);

	// asking user to provide character symbol
	printf("Enter data with char format\n");
	scanf("%c",&ch);
	
	printf("char format----->%c\n",ch);
	printf("integer format----->%d\n",ch);
	
	
	// asking user to provide character ascii
	printf("Enter data with Integer format\n");
	scanf("%d",&ch);
	
	printf("char format----->%c\n",ch);
	printf("integer format----->%d\n",ch);

	printf("%s Ends\n",__func__);
	return 0;
}
