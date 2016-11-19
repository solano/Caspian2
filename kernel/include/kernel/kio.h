#ifndef _KERNEL_KIO_H
#define _KERNEL_KIO_H

#include <stdint.h>
#include <stdarg.h>

char* itoa(uint32_t n, const uint8_t base);

void kprint(const char* str);
void kprintf(const char* str, ...);

#endif
