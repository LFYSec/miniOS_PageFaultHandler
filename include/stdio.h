/********************************************
*用户库函数声明		add by visual 2016.5.16
*************************************************/
#include "const.h"  //added by mingxuan 2019-5-19
#include "type.h" //added by mingxuan 2019-5-19

#ifndef _STDIO_H_  //added by mingxuan 2019-5-19
#define _STDIO_H_  //added by mingxuan 2019-5-19

/*syscall.asm*/
int get_ticks();
int get_pid();					
void* kmalloc(int size);			
void* kmalloc_4k();			
void* malloc(int size);			
void* malloc_4k();				
int free(void *arg);				
int free_4k(void* AdddrLin);	
int fork();			
int pthread(void *arg);	
void udisp_int(int arg);
void udisp_str(char* arg);

//added by xw
/* file system */
#define	MAX_FILENAME_LEN	12
#define	MAX_PATH	128
#define	O_CREAT		1
#define	O_RDWR		2
#define SEEK_SET	1
#define SEEK_CUR	2
#define SEEK_END	3

int open(const char *pathname, int flags);		//added by xw, 18/6/19
int close(int fd);								//added by xw, 18/6/19
int read(int fd, void *buf, int count);			//added by xw, 18/6/19
int write(int fd, const void *buf, int count);	//added by xw, 18/6/19
int lseek(int fd, int offset, int whence);		//added by xw, 18/6/19
int unlink(const char *pathname);				//added by xw, 18/6/19
//~xw

/*string.asm*/
void* memcpy(void* p_dst, void*  p_src, int size);//void* memcpy(void* es:p_dst, void* ds:p_src, int size);
void memset(void* p_dst, char ch, int size);
char* strcpy(char* p_dst, char* p_src);
int strlen(char* p_str);	//added by xw, 18/6/19

/*printf.c*/
//added by mingxuan 2019-5-19
#define EOF -1
#define _INTSIZEOF(n)   ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
#define va_start(ap,v) ( ap = (va_list)&v + _INTSIZEOF(v) )
#define va_arg(ap,t)    ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
#define va_end(ap)      ( ap = (va_list)0 )
#define isspace(s)  (s==' ')

#define TOLOWER(x) ((x) | 0x20)
#define isxdigit(c)    (('0' <= (c) && (c) <= '9') || ('a' <= (c) && (c) <= 'f') || ('A' <= (c) && (c) <= 'F'))
#define isdigit(c)    ('0' <= (c) && (c) <= '9')

PUBLIC	int	vsprintf(char *buf, const char *fmt, va_list args);
PUBLIC	int	sprintf(char *buf, const char *fmt, ...);
PUBLIC	int	printf(const char *fmt, ...); 
PUBLIC  int scanf(char *str, ...);
char getchar();         //added by mingxuan 2019-5-23
char* gets(char *str);  //added by mingxuan 2019-5-23

#endif  //added by mingxuan 2019-5-19