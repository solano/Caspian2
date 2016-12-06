#ifndef _KERNEL_INTERRUPTS_H
#define _KERNEL_INTERRUPTS_H

#include <stdint.h>

/* Types and attributes for IDT entries */
#define IDT_ATTR_PRESENT        0x80
#define IDT_ATTR_UNPRIVILEDGED  0x60 //Descriptor Priviledge Level=3
#define IDT_TYPE_TASKGATE       0x5
#define IDT_TYPE_INT16GATE      0x6
#define IDT_TYPE_TRAP16GATE     0x7
#define IDT_TYPE_INT32GATE      0xE
#define IDT_TYPE_TRAP32GATE     0xF

typedef struct {
    uint32_t offset;    // pointer to ISR
    uint16_t selector;  // GDT code selector
    uint8_t type;}      // type and attributes
idt_entry;

void pack_idt_entry(idt_entry entry, uint8_t* dest);
void* load_idt(idt_entry* descriptors, unsigned int number, uint8_t* dest);

void* kernel_load_idt(void);
void isr_keyboard(void);

#endif
