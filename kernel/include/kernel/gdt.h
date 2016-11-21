#ifndef _GDT_H
#define _GDT_H

#include <stdint.h>

typedef struct { uint32_t base, limit;
                 uint8_t type; } gdt_entry;

void load_gdt(gdt_entry GDT[], uint8_t* target, unsigned int n);

#endif
