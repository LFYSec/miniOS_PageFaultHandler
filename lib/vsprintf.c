/*************************************************************************//**
 *****************************************************************************
 * @file   vsprintf.c
 * @brief  vsprintf()
 * @author Forrest Y. Yu
 * @date   2005
 *****************************************************************************
 *****************************************************************************/

#include "type.h"
#include "stdio.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "fs.h"
#include "proc.h"
#include "tty.h"
#include "console.h"
#include "global.h"
#include "keyboard.h"
#include "proto.h"


PUBLIC char getchar();
PUBLIC char* gets(char *str);

PRIVATE char* i2a(int val, int base, char ** ps);


// TODO bug fix
PRIVATE char* i2a(int val, int base, char ** ps)
{
	int m = val % base;
	int q = val / base;
	if (q) {
		i2a(q, base, ps);
	}
	*(*ps)++ = (m < 10) ? (m + '0') : (m - 10 + 'A');

	return *ps;
}


int sprintf(char *buf, const char *fmt, ...)
{
    va_list arg = (va_list)((char*)(&fmt) + 4); 
    return vsprintf(buf, fmt, arg);
}

PUBLIC int vsprintf(char *buf, const char *fmt, va_list args)
{
	char*	p;

	va_list	p_next_arg = args;
	unsigned int	m;

	char	inner_buf[256];
	char	cs;
	int	align_nr;

	for (p=buf;*fmt;fmt++) {
		if (*fmt != '%') {
			*p++ = *fmt;
			continue;
		}
		else {		/* a format string begins */
			align_nr = 0;
		}

		fmt++;

		if (*fmt == '%') {
			*p++ = *fmt;
			continue;
		}
		else if (*fmt == '0') {
			cs = '0';
			fmt++;
		}
		else {
			cs = ' ';
		}
		while (((unsigned char)(*fmt) >= '0') && ((unsigned char)(*fmt) <= '9')) {
			align_nr *= 10;
			align_nr += *fmt - '0';
			fmt++;
		}

		char * q = inner_buf;
		memset(q, 0, sizeof(inner_buf));

		switch (*fmt) {
		case 'c':
			*q++ = *((char*)p_next_arg);
			p_next_arg += 4;
			break;
		case 'x':
			m = *((unsigned int*)p_next_arg);
			i2a(m, 16, &q);
			p_next_arg += 4;
			break;
		case 'd':
			m = *((int*)p_next_arg);
			if (m < 0) {
				m = m * (-1);
				*q++ = '-';
			}
			i2a(m, 10, &q);
			p_next_arg += 4;
			break;
		case 's':
			strcpy(q, (*((char**)p_next_arg)));
			q += strlen(*((char**)p_next_arg));
			p_next_arg += 4;
			break;
		default:
			break;
		}

		int k;
		for (k = 0; k < ((align_nr > strlen(inner_buf)) ? (align_nr - strlen(inner_buf)) : 0); k++) {
			*p++ = cs;
		}
		q = inner_buf;
		while (*q) {
			*p++ = *q++;
		}
	}

	*p = 0;

	return (p - buf);
}

PUBLIC char getchar(){
	char ch;
	return read(STD_IN,&ch,1)==1 ? ch : EOF;
}

PUBLIC char* gets(char *str){
	char c;
	char *cs;
	cs= str;
	while( (c=getchar()) != EOF ){
		if( (*cs=c)=='\n' ){
			*cs = '\0';
			break;
		}
		cs++;
	}
	return str;
}

unsigned long strtoul(const char *cp,char **endp,unsigned int base)
{
    unsigned long result = 0,value;

    if (!base) {
        base = 10;
        if (*cp == '0') {
            base = 8;
            cp++;
            if ((TOLOWER(*cp) == 'x') && isxdigit(cp[1])) {
                cp++;
                base = 16;
            }
        }
    } else if (base == 16) {
        if (cp[0] == '0' && TOLOWER(cp[1]) == 'x')
            cp += 2;
    }
    while (isxdigit(*cp) &&
           (value = isdigit(*cp) ? *cp-'0' : TOLOWER(*cp)-'a'+10) < base) {
        result = result*base + value;
        cp++;
    }
    if (endp)
        *endp = (char *)cp;
    return result;
}


long strtol(const char *cp,char **endp,unsigned int base)
{
    if(*cp=='-')
        return -strtoul(cp+1,endp,base);
    return strtoul(cp,endp,base);
}
int  atoi(const     char  *nptr)
{
    return  strtol(nptr, (    char  **)NULL, 10);
}
