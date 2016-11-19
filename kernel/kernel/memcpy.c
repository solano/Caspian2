#include <string.h>

void* memcpy(void *dstptr, const void *srcptr, size_t n) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;
    for (unsigned int i=0; i<n; i++)
        dst[i] = src[i];
    return dstptr;
}
