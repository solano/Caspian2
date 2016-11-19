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

/* Text mode provided by hardware */
#include <kernel/tty.h>
#include <kernel/kio.h>

/* Proper kernel starts */
void kernel_main(void) {
    term_init();
    kprint("Hello, world!\n");
    kprintf("TEST: %x in hexadecimal means %u in decimal", 1234, 1234);
}
