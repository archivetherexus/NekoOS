; This file implements the gdt_flush function
; used by "gdt.c".

; Wiki: http://wiki.osdev.org/GDT_Tutorial
; Wiki: http://wiki.osdev.org/CPU_Registers_x86#Segment_Registers

global gdt_flush

; Declared in "gdt.c".
extern gdt_pointer

gdt_flush:
	lgdt [gdt_pointer]	; Load the GDT via the gdt_pointer.
	jmp 0x08:flush		; Update the segment registers.
flush:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret
