
char* strcat(char *dst, const char *src)
{
    if(dst==0 || src==0) return 0;
    char *temp = dst;
    while (*temp != '\0')
        temp++;
    while ((*temp++ = *src++) != '\0');

    return dst;
}