
/*======================================================================*
                          Syscall Fork Test
added by xw, 18/4/27
 *======================================================================*/
#include "stdio.h"
int global=0;
char *str2,*str3;

void main(int arg,char *argv[])
{
	int i=0;
	
	//i = get_pid();

	printf("Hello world!\n");
	/* //deleted by mingxuan 2019-5-23
	int pid = fork();
		while(1)
		{
			printf("init");
			printf("%d",++global);
			printf(" ");
			i=10000;
			while(--i);
		}
	*/
	while (1);

	return ;
}