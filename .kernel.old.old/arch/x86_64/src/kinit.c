/*
 * This part of the kernel initialises the arch specific stuff...
 */

#include <stdint.h>
#include "multiboot.h"
#include "gdt.h"
#include "libk/string.h"
#include "kdrivers/vga.h" // < Temporary

// FIXME: This is ugly. Fix in the future!
extern void kmain();

/**
 * kinit is called by the bootstrap.
 *
 * The args are pushed by the bootstrap code.
 */
void kinit(struct multiboot_info *mboot_ptr, uint32_t mboot_magic)
{

	/* Check that we are being loaded correctly. */
	if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
		vga_clear_screen();
		terminal_write_string("ERROR: magic number did not match...");
		return;
	}

	 /* First thing to initialise is the global descriptor table. */
	gdt_init();

	vga_clear_screen();

	/* Print bootloader name. */

	// FIXME: Implement printf to make this less messy.
	terminal_write_string("Bootloader: ");
	terminal_write_string((char*)mboot_ptr->boot_loader_name);
	terminal_row++;
	terminal_column = 0;

	uint32_t available_mem = mboot_ptr->mem_upper;
	terminal_write_string("Available memmory: ");
	char buffer[32];
	itoa(available_mem, buffer, 10);
	terminal_write_string(buffer);
	terminal_row++;
	terminal_column = 0;

	/*
	 * All x86 specific drivers have been initialized. 
	 * Now call the generic main!
	 */
	kmain();
}


