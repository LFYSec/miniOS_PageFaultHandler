#ifndef _SHELL_H
#define _SHELL_H

#define _NR_ARGV 10
#define LENGTH_OF_ARGV 256

int fd;
int argc;
char argv[_NR_ARGV][LENGTH_OF_ARGV];

void split(char *s);
void strncpy(char *s1, char *s2, int len);
static strcmp(const char *s1, const char *s2);

#endif