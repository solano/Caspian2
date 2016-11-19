#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

size_t strlen(const char* str);

void* memcpy(void *destptr, const void *srcptr, size_t n);

char* reverse(char str[]);

#endif
