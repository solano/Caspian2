/* Freestanding part of the C library (which I don't need to write) */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Sanity checks */
#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif
#if !defined(__i386__)
#error "This needs to be compiled with an ix86-elf compiler"
#endif

#include <kernel/tty.h>
#include <kernel/kio.h>
#include <kernel/gdt.h>
#include <kernel/interrupts.h>

gdt_entry GDT[3] =
{   {.base=0, .limit=0,          .type=0},      // Selector 0x00 can't be used
    {.base=0, .limit=0xffffffff, .type=0x9A},   // Code
    {.base=0, .limit=0xffffffff, .type=0x92}};  // Data
uint8_t GDT_encoded[3*8]; // each entry takes 8 bytes

void kernel_main(void) {
    load_gdt(GDT, GDT_encoded, 3);
    term_init();

    kprintf("IDT is in %x\n", (uint32_t) kernel_load_idt());

    kprint("Hello, world!\n");
    kprintf("TEST: %x in hexadecimal means %u in decimal\n", 1234, 1234);
    isr_keyboard();
}
