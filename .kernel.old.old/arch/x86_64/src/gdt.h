/**
 * This file contains global descriptor stuff.
 * Wiki: http://wiki.osdev.org/Global_Descriptor_Table
 */

#pragma once

/** Defines a GDT entry. */
struct gdt_entry {
	unsigned short limit_low;
	unsigned short base_low;
	unsigned char base_middle;
	unsigned char access;
	unsigned char granularity;
	unsigned char base_high;
} __attribute__((packed));

/** Used to load in the GDT with the lgdt instruction. */
struct gdt_ptr {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

/** A GDT with 3 entries. */
struct gdt_entry gdt[3];

/** The GDT pointer. */
struct gdt_ptr gdt_pointer;

/**
 * Flushes changes made to the gdt.
 * Updates the segment registers.
 */
extern void gdt_flush();

/**
 * Initialises the default gdt setup.
 * Should be called by kinit!
 */
void gdt_init();

/**
 * Used to change the GDT.
 * Remember to use: gdt_flush() after changes.
 */
void gdt_set_gate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
