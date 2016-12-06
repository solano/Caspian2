;;; Implements load_idtr, used by interrupts.c, and a few
;;; Interrupt Service Routines (ISRs).

section .data
idtr:
        dw 0    ; limit
        dd 0    ; base
dummy:
        dw 0
        dd 0

section .text

global load_idtr:function
; Loads an IDT's address and limit into the IDTR register.
; According to the IDT manual, the limit should be 8N-1, where
; N is the number of descriptors.
; Returns a pointer to a stored version of the IDTR.
; void* load_idtr(uint8_t* ptr, size_t limit)
load_idtr:
    mov eax, [esp+4]
    mov [idtr+2], eax
    mov ax, [esp+8]
    mov [idtr], ax
    lidt [idtr]
    sidt [dummy]
    mov eax, [dummy+2]
    ret

;;;;;; ISRs ;;;;;;

extern ktest
extern pic_master_eoi
extern pic_slave_eoi

global isr_keyboard:function
; void isr_keyboard(void)
isr_keyboard:
    call ktest
    call pic_master_eoi
    ;iret
