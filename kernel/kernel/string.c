#include <stddef.h>

#include <string.h>

/* NOT IN STANDARD LIBC
 * Reverses string in place and returns it */
char* reverse(char* str) {
    int i, j;
    char c;

    for (i=0, j=strlen(str)-1; i < j; i++, j--) {
        c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
    return str;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while(str[len])
        len++;
    return len;
}

void* memcpy(void *dstptr, const void *srcptr, size_t n) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;
    for (unsigned int i=0; i<n; i++)
        dst[i] = src[i];
    return dstptr;
}

inline char* strcpy(char* restrict s1, const char* restrict s2) {
    return memcpy(s1, s2, strlen(s1));
}
