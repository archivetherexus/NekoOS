/************************************************
 *		 The NekoOS source code			   *
 *		 By: Tyrerexus						*
 *		 Date: 31 July 2017				   *
 ************************************************/

///======================================
/// OS Config
///======================================

#define SYSTEM_NAME "NekoOS"
#define SYSTEM_VERSION "0.1"

///======================================
/// Other headers for NekoOS.
///======================================

/// The library functions.
#include "lib.h"

/// The NekoScripkt interpreter.
#include "neko_script.h"

/// The FAT32 filesystem.
//#include "fat32.h"

///======================================
/// Operating system implmentation.
///======================================
/// Skip this step if a stdlib is present.
#ifndef SIMULATE

//
// VGA DRIVER
//

/// Consts & Variables.

/** Max VGA width. */
static const size_t VGA_WIDTH = 80;

/** Max VGA height. */
static const size_t VGA_HEIGHT = 25;

/** Current row. */
size_t vga_terminal_row;

/** Current column. */
size_t vga_terminal_column;

/** Current VGA color. */
uint8_t vga_terminal_color;

/** A pointer to the VGA buffer. */
uint16_t* vga_terminal_buffer;

/// Functions. 

static void vga_terminal_putentryat(char character, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	vga_terminal_buffer[index] = (uint16_t) character | (uint16_t) color << 8;
}
 
static void vga_terminal_putchar(char c) {
	vga_terminal_putentryat(c, vga_terminal_color, vga_terminal_column, vga_terminal_row);
	if (++vga_terminal_column == VGA_WIDTH) {
		vga_terminal_column = 0;
		if (++vga_terminal_row == VGA_HEIGHT)
			vga_terminal_row = 0;
	}
}

/** Prepares the VGA driver for usage. */
void vga_driver_init() {
	/* Move cursor position. */
	vga_terminal_row = 0;
	vga_terminal_column = 0;

	/* Black color with blue background. */
	vga_terminal_color = 0x10;

	vga_terminal_buffer = (uint16_t*) 0xB8000;

	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			vga_terminal_buffer[index] = (uint16_t) ' ' | (uint16_t) vga_terminal_color << 8;
		}
	}
}

//
// Kernel stuff
//

/// The main function. 

/** This functions is called from the bootup assembly. */
void kmain() {
	/* Init vga. */
	vga_driver_init();
	puts("Welcome to NekoOS v0.1");

	/* Done. */
	__asm__ __volatile__("cli");
	while(1);
}

//=======================================
// Implement the lib.h functions.
//=======================================

int putchar(char c) {
	vga_terminal_putchar(c);
	return c;	
}

int puts(char *str) {
	while(*str != 0)
		putchar(*str++);
	putchar('\n');
	return 1;
}

int getchar() {
	// FIXME: Not implemented yet...
	return '\0';
}

size_t strlen(const char *str) {
	const char *begin = str;
	while(*str++)
		;
	return str - begin;
}

int strcmp (const char *str1, const char *str2) {
	while(*s1 && (*s1 == *s2)) {
		s1++;
		s2++;
	}
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

//=======================================
// Run on *NIX.
//=======================================
/// If stdlib is present, use it's implementation instead.
#else

int printf(const char *str, ...);

int main() {
	ScriptRuntime *runtime   = malloc(sizeof(ScriptRuntime));
	runtime->instructions    = calloc(10, sizeof(size_t));
	runtime->arguments       = calloc(10, sizeof(size_t));
	runtime->stack           = calloc(10, sizeof(size_t));
	runtime->stack_pointer   = runtime->stack + (10 - 1);
	runtime->stack_top       = 0;
	runtime->program_counter = 0;
	runtime->stopped		 = false;

	runtime_insert_instruction(runtime, string_to_instruction("push"), 3);

	runtime->instructions[1] = string_to_instruction("push");
	runtime->arguments   [1] = 7;

	runtime->instructions[2] = string_to_instruction("add");
	
	runtime->program_counter = 3;
	runtime_load_script(runtime);
	runtime_insert_instruction(runtime, string_to_instruction("add"), 0);
	runtime->program_counter++;
	runtime->instructions[runtime->program_counter] = &instruction(stop);

	runtime->program_counter = 0;
	

	while(!runtime->stopped) {
		runtime_step(runtime);
	}
	
	printf("Sum: %d", runtime_pop(runtime));

	free(runtime->instructions);
	free(runtime->arguments);
	free(runtime->stack);
	free(runtime);

	return 0;
}

#endif


