#include "stdio.h"
/*======================================================================*
                           Syscall Sleep Test
added by xw, 18/8/16
 *======================================================================*/
void main(int arg,char *argv[])
{
	int i=0;
	
	while(1)
	{
		printf("U( ");
		printf("[");
		printf("%d",get_ticks());
		printf("] ");
		sleep(5);
		printf("[");
		printf("%d",get_ticks());
		printf("] ");
		printf(") ");
		i=10000;
		while(--i){}
	}
	return ;
}