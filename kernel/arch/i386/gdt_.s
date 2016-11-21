section .data:
gdtr dw 0   ; For limit storage
     dd 0   ; For base storage

section .text:
global load_encoded_gdt

; Called by load_gdt.
; Tells the CPU where is the Global Descriptor Table.
; void load_encoded_gdt(uint8_t* packed_gdt, uint16_t size)
load_encoded_gdt:
    mov eax, [esp+4]
    mov [gdtr+2], eax
    mov ax, [esp+8]
    mov [gdtr], ax
    lgdt [gdtr]
    ret
