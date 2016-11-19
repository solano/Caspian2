; Declare constants for the multiboot header
MBALIGN equ 1<<0                ; align loaded modules on page boundaries
MEMINFO equ 1<<1                ; provide memory map
FLAGS equ MBALIGN | MEMINFO     ; 'flag' field
MAGIC equ 0x1BADB002            ; lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above

; Header that marks this program as a kernel.
; These magic values are documented in the multiboot standard.
; The bootloader will search for this signature in the first 8 KiB of
; the kernel file, aligned at a 32-bit boundary. The signature is in
; its own section so the header can be forced to be within the first
; 8 KiB of the kernel file.
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

; Providing a stack
section .bss
align 4
stack_bottom:
resb 16384     ; 16 KiB
stack_top:

; The bootloader will jump to this position once the kernel has been
; loaded. As the bootloader is gone, it doesn't make sense to return
; from this function.
section .text
global _start:function (_start.end - _start)

_start:
        ; We're now in 32-bit protected mode on a x86 machine. Interrupts
        ; and paging are disabled. The kernel has full control of the CPU.
        ; It can only make use of code it provides itself.
        
        ; Setting up a stack
        mov esp, stack_top

        ; TODO: Enable paging
        ; TODO: Load GDT

        ; Before calling the high-level kernel, the ABI requires our stack
        ; to be 16-byte aligned. We've pushed 0 bytes to it, a multiple of
        ; 16, so we're ready to go.
        extern kernel_main
        call kernel_main

        ; Nothing more to do? Do an infinite loop!
        cli     ; Disable interrupts
.hang:  hlt     ; Wait for interrupts. Poor CPU
        jmp .hang  ; In case it ever wakes up
.end:
