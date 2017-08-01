#!/bin/sh

# Set and create the out dir.
OUT_DIR=out
mkdir -p $OUT_DIR

## Creates the kernel.bin.
function kernel
{

# Create the multiboot.asm.
cat > $OUT_DIR/multiboot.asm << 'END_OF_MULTIBOOT'
; Declare constants for the multiboot header.
MBALIGN  equ  1<<0              ; Align loaded modules on page boundaries.
MEMINFO  equ  1<<1              ; Provide memory map.
FLAGS    equ  MBALIGN | MEMINFO ; This is the Multiboot 'flag' field.
MAGIC    equ  0x1BADB002        ; 'Magic number' lets bootloader find the header.
CHECKSUM equ -(MAGIC + FLAGS)   ; Checksum of above, to prove we are multiboot.
 
; Put these values into the multiboot section with an alignment of 4-bytes.
section .multiboot
align 4
	dd MAGIC
	dd FLAGS
	dd CHECKSUM


section .bss

; Align the stack by 4
align 4
stack_bottom:
; Reserce 16KiB for our stack.
resb 16384 ; 16 KiB
stack_top:

section .text

; Setup stack and call main.
global _start:function (_start.end - _start)
_start:
	mov esp, stack_top
	extern kmain
	call kmain
.end:
END_OF_MULTIBOOT

# Create linker.ld.
cat > $OUT_DIR/linker.ld << 'END_OF_LINKER'
/* What to start at kernel boot-up. */
ENTRY(_start)

SECTIONS
{
	/* Begin putting sections at 1 MiB. */
	. = 1M;

	/* Multiboot header comes first. */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}

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
}
END_OF_LINKER

# Compile the main c file.
gcc -c kernel.c -o $OUT_DIR/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32 -static
if [ $? -ne 0 ]; then
	return 1
fi

# Compile the multiboot.
nasm -f elf32 -o $OUT_DIR/multiboot.o $OUT_DIR/multiboot.asm
if [ $? -ne 0 ]; then
	return 1
fi

# Link the files.
ld -n -melf_i386 -T $OUT_DIR/linker.ld -o $OUT_DIR/kernel.bin $OUT_DIR/kernel.o $OUT_DIR/multiboot.o
if [ $? -ne 0 ]; then
	return 1
fi

# All commands succeded!
return 0;
}

# Creates the bootable.iso.
function iso
{
	# Assure that the directories exist.
	mkdir -p $OUT_DIR/isodir/boot/grub

	# Create the grub.cfg file.
cat > $OUT_DIR/isodir/boot/grub/grub.cfg << 'END_OF_GRUB'

menuentry "NekoOS" {
	multiboot /boot/kernel.bin	
}

END_OF_GRUB

	# Compy the kernel.bin 
	cp $OUT_DIR/kernel.bin $OUT_DIR/isodir/boot/kernel.bin
	if [ $? -ne 0 ]; then
		return 1
	fi

	# Create the iso.
	grub-mkrescue --xorriso=xorriso -o $OUT_DIR/bootable.iso $OUT_DIR/isodir 2> /dev/null
	if [ $? -ne 0 ]; then
		return 1
	fi

	# All commands succeded!
	return 0
}


# Create the kernel.bin.
# Then create the bootable.iso which contains grub and kernel.bin.
# Then copy the bootable.iso away from the out_dir.
kernel && iso && cp $OUT_DIR/bootable.iso ../nekoo.iso


# Clean.
rm -r $OUT_DIR
