#include "stdio.h"

void main(int arg,char *argv[])
{
	int stdin = open("dev_tty2",O_RDWR);
	int stdout= open("dev_tty2",O_RDWR);
	int stderr= open("dev_tty2",O_RDWR);
	
    char buf[1024];
    int pid;
	while(1)
	while(1)
	{
        printf("MiniOs:/ $ ");
        if(gets(buf) && strlen(buf)!=0 ){
            if(exec(buf)!=0){
                printf("exec failed: file not found!\n");
            }
        }
        
	}
	return ;
}