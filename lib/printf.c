/*************************************************************************//**
 *****************************************************************************
 * @file   printf.c
 * @brief  
 * @author Forrest Y. Yu
 * @date   Wed May 14 15:50:08 2008
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "stdio.h"
#include "const.h"


/*****************************************************************************
 *                                printf
 *****************************************************************************/
/**
 * The most famous one.
 *
 * @note do not call me in any TASK, call me in USER PROC.
 * 
 * @param fmt  The format string
 * 
 * @return  The number of chars printed.
 *****************************************************************************/
PUBLIC int printf(const char *fmt,...){
    
    
    int i;
    char buf[256];
    va_list arg = (va_list)((char*)(&fmt) + 4); 
    
	i = vsprintf(buf, fmt, arg);
    
    int n = write(STD_OUT,buf,i);
    if(n==i){
        return i;
    }else{
        return -1;
    }
}


