;;; The Global Descritptor Table.
extern gdt

;;; The Interrupt Gate Descriptor.
extern idt

;;; Protected mode main.
extern pmain

;;; Functions defined here.
global pmode_init
global a20_init
global idt_load
global gdt_load

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Enter protected mode.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
pmode_init:
	cli             ; Disable interrupts.
	lgdt [gdt]      ; Load GDT register with start address of Global Descriptor Table.

	mov eax, cr0    ; Enable protected mode.
	or al, 1        ; Set PE (Protection Enable) bit in CR0 (Control Register 0).
	;inc eax
	;or eax, 1
	mov cr0, eax    ; Load the new control register value.
 
	; Perform far jump to selector 08h (offset into GDT, pointing at a 32bit PM code segment descriptor)
	; to load CS with proper PM32 descriptor).
 
	mov ax, 0x10 ; Load DS, ES, FS, GS, SS, ESP.
    	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x8:complete_flush
	complete_flush:
	call pmain
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Enable the A20 line.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
a20_init:
        cli
 
        call    a20wait
        mov     al,0xAD
        out     0x64,al
 
        call    a20wait
        mov     al,0xD0
        out     0x64,al
 
        call    a20wait2
        in      al,0x60
        push    eax
 
        call    a20wait
        mov     al,0xD1
        out     0x64,al
 
        call    a20wait
        pop     eax
        or      al,2
        out     0x60,al
 
        call    a20wait
        mov     al,0xAE
        out     0x64,al
 
        call    a20wait
        sti
        ret
a20wait:
        in      al,0x64
        test    al,2
        jnz     a20wait
        ret
a20wait2:
        in      al,0x64
        test    al,1
        jz      a20wait2
        ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load the IDT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
idt_load:
	lidt [idt]
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load the GDT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
gdt_load:
	lgdt [gdt]
	mov ax, 0x10 ; Load DS, ES, FS, GS, SS, ESP.
    	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x8:flush
	flush:
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Some interrupt handlers!
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
global interrupt_keyboard_handler
extern some_handler
interrupt_keyboard_handler:
	pushad
	cld ; C code following the sysV ABI requires DF to be clear on function entry
	call some_handler
	popad
	iret
