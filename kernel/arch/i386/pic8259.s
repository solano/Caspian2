;; This is written according to the PIC 8259A documentation.
;; Notation: Dn = nth bit, from lowest to highest significance
;;           [thing in brackets] = enabled when this bit is 0

;;;;;; I/0 addresses ;;;;;;

PIC1     equ 0x20   ; Master PIC
PIC2     equ 0xA0   ; Slave PIC
PIC1CMD  equ PIC1
PIC2CMD  equ PIC2
PIC1DATA equ PIC1+1
PIC2DATA equ PIC2+1

;;;;;; Command codes ;;;;;;

;; Initialization Command Words (ICWs) ;;

; ICW1 initializes the PICs and is always followed by ICW2.
ICW1_ICW4        equ 0x01  ; D0: ICW4 [not] needed
ICW1_SINGLE      equ 0x02  ; D1: single PIC [cascade mode]
ICW1_INTERVAL4   equ 0x04  ; D2: interval of 4 [8]
ICW1_LEVEL       equ 0x08  ; D3: level [edge] triggered mode
ICW1_INIT        equ 0x10  ; D4: must be enabled
; ICW1's D5-D7 are for MCS-80/85 mode only; we're not using them.

; ICW2 tells the PIC its new vector offset (hardcoded below)
; ICW3 is about the master-slave configuration (hardcoded below)

; ICW4 tells additional information to the PIC.
ICW4_MICROPM     equ 0x01  ; D0: 8086/88 [MCS-80/85] mode
ICW4_AUTOEOI     equ 0x02  ; D1: auto [normal] EOI
ICW4_SLAVE       equ 0x08  ; D3=1, D2=0: buffered slave mode
ICW4_MASTER      equ 0x0c  ; D3=1, D2=1: buffered master mode
ICW4_SFNM        equ 0x10  ; D4: special fully nested mode
; ICW4's D5-D7 are not used.

;; Operation Command Words (OCWs) ;;

; OCW1 sets masks on IR pins
; OCW2 takes the IR line (on specific operations) on D0-D2
; and commands on D5-D
EOI         equ 0x20 ; D5: End Of Interrupt
SPECIFIC    equ 0x40 ; D6: whether this is specific to an IR line
ROTATE      equ 0x80 ; D7: makes this the bottom-priority IR
S_EOI       equ SPECIFIC+EOI
S_ROT       equ SPECIFIC+ROTATE
S_R_EOI     equ SPECIFIC+ROTATE+EOI
; OCW3 sets the Special Mask Mode (which I do not intend to use)
; and is also used for polling (which is against the whole point of
; having an interrupt-driven system).

;;;;;; Hardcoded values ;;;;;;

;; Vector offsets for the PICs (sent in ICW2)
OFFSET1 equ 0x20    ; Just after the processor's reserved vectors
OFFSET2 equ 0x28

;; Master-slave configuration of the Industry Standard Architecture (ISA)
;; (note: ISA (1981) is obsolete, superseded by PCI in 1993)
;; this is sent in ICW3
MASTER_PIC  equ 0x04    ; IR2 used for cascading
SLAVE_PIC   equ 0x02    ; Tells slave its ID

;;;;;; End of settings ;;;;;;

section .bss
m1: resb 1      ; m1 and m2 will store masks for preservation
m2: resb 1

section .text

global pic_init:function
; Initializes the PICs and returns current masks.
; uint16_t pic_init(void)
pic_init:
    ; save masks before doing anything
    mov edi, m1
    mov dx, PIC1DATA
    insb
    mov edi, m2
    mov dx, PIC2DATA
    insb

    ; start the initialization sequence (ICW1 on both PICs)
    mov al, ICW1_INIT+ICW1_ICW4     ; cascade, edge-triggered,
    out PIC1CMD, al                 ; intervals of 8, ICW4 required
    out PIC2CMD, al

    ; send vector offsets (ICW2)
    mov al, OFFSET1
    out PIC1DATA, al
    mov al, OFFSET2
    out PIC2DATA, al

    ; send master-slave configuration (ICW3)
    mov al, MASTER_PIC
    out PIC1DATA, al
    mov al, SLAVE_PIC
    out PIC2DATA, al

    ; additional settings (ICW4)
    mov al, ICW4_MICROPM     ; 8086/88 mode, no buffering, normal EOI
    out PIC1DATA, al        ; and no Special Fully Nested Mode
    out PIC2DATA, al

    ; restore masks (OCW1)
    mov al, [m1]
    out PIC1DATA, al
    mov al, [m2]
    out PIC2DATA, al

    mov ah, [m1]      ; return masks
    ret

global irq_set_mask:function
; The nth bit (from lowest to highest) will mask the IRQ #n,
; thus preventing it from generating interrupts.
; void irq_set_mask(uint16_t mask)
irq_set_mask:
    mov ax, [esp+4]
    out PIC2DATA, al
    mov al, ah
    out PIC1DATA, al
    ret

global pic_master_eoi:function
; Tells the PIC an Interrupt Service Routine has ended (and now
; it can raise new IRQs) and returns from ISR.
; void pic_master_eoi(void)
pic_master_eoi:
    mov al, EOI
    out PIC1CMD, al
    ret

global pic_slave_eoi:function
; Same as above, but used in slave IRQs.
; void pic_slave_eoi(void)
pic_slave_eoi:
    mov al, EOI
    out PIC2CMD, al
    out PIC1CMD, al
    ret
