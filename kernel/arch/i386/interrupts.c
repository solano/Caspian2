#include <kernel/interrupts.h>

#include <stdint.h>
#include <string.h>

/* Currently implemented for the 8259 PIC only */
extern uint16_t pic_init(void);

extern void* load_idtr(uint8_t* ptr, size_t limit);

/* Packs an idt_entry structure and writes it to dest. */
void pack_idt_entry(idt_entry entry, uint8_t* dest) {
    uint16_t offset1 = entry.offset && 0xFFFF; // 0..15 bits
    uint16_t offset2 = entry.offset >> 16;     // 16..31 bits
    uint8_t zero = 0;
    
    memcpy(dest, &offset1, 2);
    memcpy(dest+2, &(entry.selector), 2);
    memcpy(dest+4, &zero, 1);            // zeroed-out byte (unused)
    memcpy(dest+5, &(entry.type), 1);
    memcpy(dest+6, &offset2, 2);
}

void* load_idt(idt_entry *descriptors, unsigned int number, uint8_t* dest) {
    pic_init();
    for(unsigned int i=0; i<number; i++)
        pack_idt_entry(descriptors[i], dest+8*i);
    return load_idtr(dest, 8*number-1);
}

/* Everything up to this point is generic and can be used to, say,
 * load a new interrupt descriptor or load a new IDT in real-time
 * (for whichever purpose).
 *
 * What follows is the actual implementation of the IDT kernel_main
 * will first load, which could be done in kernel.c but I found
 * more elegant to write here. */

#include <kernel/kio.h>

#define IDT_LAST_DESCRIPTOR     0x40
#define IDT_DESC_QUANTITY       IDT_LAST_DESCRIPTOR+1
#define IRQ_OFFSET              0x20 // also hardcoded in pic8259.s

/* Reserving space for the IDT. Each entry takes 8 bytes. */
uint8_t IDT_packed[IDT_DESC_QUANTITY*8];

idt_entry IDT[IDT_DESC_QUANTITY];

extern void isr_keyboard(void);

void* kernel_load_idt(void) {
    /* IRQ 1: keyboard */
    IDT[IRQ_OFFSET+1].offset = (uint32_t) &isr_keyboard;
    IDT[IRQ_OFFSET+1].selector = 0x01;          // code selector
    IDT[IRQ_OFFSET+1].type = IDT_ATTR_PRESENT
                            +IDT_ATTR_UNPRIVILEDGED
                            +IDT_TYPE_INT32GATE;
    kprintf("Loading IDT to %x\n", (uint32_t) IDT_packed);
    return load_idt(IDT, IDT_DESC_QUANTITY, IDT_packed);
}

