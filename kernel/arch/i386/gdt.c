#include <kernel/gdt.h>

#include <stdint.h>
#include <kernel/kio.h>

extern void load_encoded_gdt(uint8_t* packed_gdt, uint16_t size);

/* Function from OSDev Wiki. Didn't bother writing myself */
static void encode_gdt_entry(uint8_t* target, gdt_entry source) {
    // Check the limit to make sure that it can be encoded
    if ((source.limit > 65536) && (source.limit & 0xFFF) != 0xFFF) {
        kprint("You can't do that!");
    }
    if (source.limit > 65536) {
        // Adjust granularity if required
        source.limit = source.limit >> 12;
        target[6] = 0xC0;
    } else {
        target[6] = 0x40;
    }
 
    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] |= (source.limit >> 16) & 0xF;
 
    // Encode the base 
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;
 
    // And... Type
    target[5] = source.type;
}

void load_gdt(gdt_entry GDT[], uint8_t* target, unsigned int n) {
    for(unsigned int i=0; i<n; i++)
        encode_gdt_entry(target+8*i, GDT[i]);
    load_encoded_gdt(target, 8*n);
}
