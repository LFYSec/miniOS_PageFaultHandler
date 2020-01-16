#include "stdio.h"
/*======================================================================*
                           Syscall Yield Test
added by xw, 18/8/16
 *======================================================================*/
void main(int arg,char *argv[])
{
	int i=0;
	
	while(1)
	{
		printf("U( ");
		yield();
		printf(") ");
		i=10000;
		while(--i){}
	}
	return ;
}

// void main(int arg,char *argv[])
// {
// 	int i=0;
	
// 	while(1)
// 	{
// 		udisp_str("U( ");
// 		//yield();
// 		udisp_str(") ");
// 		i=10000;
// 		while(--i){}
// 	}
// 	return ;
// }

