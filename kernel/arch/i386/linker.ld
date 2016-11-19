/* The bootloader will start our execution here */
ENTRY(_start)

SECTIONS
{
    /* Puts the kernel at 1 MiB (conventional) */
    . = 1M;

    /* Multiboot header first, or the bootloader won't recognize it */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /* Read-only data */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /* Read-write data (initialized) */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }

    /* Add more sections here as needed */
}
