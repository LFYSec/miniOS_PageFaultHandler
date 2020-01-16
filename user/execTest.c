/******************************************************
*	init进程	add by visual 2016.5.16
*******************************************************/

#include "stdio.h"
int global=0;
char *str2,*str3;
void main(int arg,char *argv[])
{
	int i=0;
	
	//printf("%d",get_pid());
	// while(1);
	
	while(1)
	{
		printf("init");
		printf("%d",++global);
		printf(" ");
		i=10000;
		while(--i){}
	}
	return ;
}