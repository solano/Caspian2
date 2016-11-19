#include <stddef.h>

#include <string.h>

size_t strlen(const char* str) {
    size_t len = 0;
    while(str[len])
        len++;
    return len;
}

/* reverse string in place */
char* reverse(char str[]) {
    int i, j;
    char c;

    for (i=0, j=strlen(str)-1; i < j; i++, j--) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
    return str;
}
