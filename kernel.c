/************************************************
 *       The NekoOS source code                 *
 *       By: Tyrerexus                          *
 *       Date: 31 July 2017                     *
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

/// The uint*_t types.
#include <stdint.h>

///======================================
/// Operating system implmentation.
///======================================
/// Skip this step if a stdlib is present.
#ifndef SIMULATE

static inline void outb(uint16_t port, uint8_t val)
{
	asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile ("inb %1, %0"
					: "=a"(ret)
					: "Nd"(port));
	return ret;
}

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
	if (c == '\n') {
		++vga_terminal_row;
		vga_terminal_column = 0;
		return;
	}
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
// IDT driver.
//

#define IDT_INTERRUPT_GATE 0x8E
#define IDT_TRAP_GATE      0x8F

typedef struct idt_entry_t {
   uint16_t offset_low;  // Offset bits 0..15.                     //
   uint16_t selector;    // A code segment selector in GDT or LDT. //
   uint8_t zero;         // Unused, set to 0.                      //
   uint8_t type_attr;    // Type and attributes, see below.        //
   uint16_t offset_mid;  // Offset bits 16..31.                    //
   uint32_t offset_high;
   uint32_t reserved;
} idt_entry;

typedef struct idt_table_t {
	uint16_t size;
	uint32_t offset;
} __attribute__((__packed__)) idt_table;

void idt_load();
idt_table idt;
idt_entry idt_entries[256];


void idt_entry_set(idt_entry *entry, uint64_t offset, uint16_t selector, uint8_t type_attr) {
	entry->offset_low   = (uint16_t)offset;
	entry->offset_mid   = (uint16_t)(offset >> 16);
	entry->offset_high  = (uint16_t)(offset >> 32);

	entry->selector  = selector;
	
	entry->type_attr = type_attr;
	
	entry->zero     = 0;
	entry->reserved = 0;
}



void some_handler() {
	puts("Kek, we got so far!");
}

extern void interrupt_keyboard_handler();

static void remap_irqs()
{
    // Remap IRQ table. //
    outb(0x20, 0x11);
    outb(0xa0, 0x11);
    outb(0x21, 0x20);
    outb(0xa1, 0x28);
    outb(0x21, 0x04);
    outb(0xa1, 0x02);
    outb(0x21, 0x01);
    outb(0xa1, 0x01);
    outb(0x21, 0x00);
    outb(0xa1, 0x00);
}

void idt_driver_init() {
	idt.size   = (sizeof(idt_entry) * 256) - 1;
	idt.offset = (uint32_t)&idt_entries; 

	//for (int i = 0; i <= 47; i++) {
	//	idt_entry_set(&idt_entries[i], (uint32_t)interrupt_keyboard_handler, 0x08, IDT_INTERRUPT_GATE);
	//}

	//idt_entry_set(&idt_entries[1], (uint32_t)interrupt_keyboard_handler, 0x08, IDT_INTERRUPT_GATE);
	idt_entry_set(&idt_entries[33], (uint32_t)interrupt_keyboard_handler, 0x08, IDT_INTERRUPT_GATE);
	idt_entry_set(&idt_entries[21], (uint32_t)interrupt_keyboard_handler, 0x08, IDT_INTERRUPT_GATE);

	idt_load();
	
	//asm("int $21");
}

//
// GDT driver.
//


// https://en.wikibooks.org/wiki/X86_Assembly/Global_Descriptor_Table
typedef struct gdt_entry_t {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_middle;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  base_high;
} __attribute__((packed)) gdt_entry;

typedef struct gdt_table_t {
	uint16_t size;
	uint32_t offset;
} __attribute__((__packed__)) gdt_table;

void gdt_load();
gdt_table gdt;   
gdt_entry gdt_entries[5];

void gdt_entry_set(gdt_entry *entry, uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity) {
	entry->base_low = (base & 0xffff);
	entry->base_middle    = (base >> 16) & 0xff;
	entry->base_high      = (uint8_t)(base >> 24) & 0xff;

	entry->limit_low      = (limit & 0xffff);
	entry->granularity    = (limit >> 16) & 0x0f;

	entry->granularity    = (uint8_t)(entry->granularity | (granularity & 0xf0));
	entry->access         = access;
}

void gdt_driver_init() {
	gdt.size   = (sizeof (gdt_entry) * 5) - 1;
	gdt.offset = (uint32_t)&gdt_entries;
	
	
	// Null segment. //
	gdt_entry_set(&gdt_entries[0], 0, 0,          0,    0   );
	 
	// Kernel code segment. //
    gdt_entry_set(&gdt_entries[1], 0, 0xffffffff, 0x9a, 0xcf);
    
    // Kernel data segment. //
    gdt_entry_set(&gdt_entries[2], 0, 0xffffffff, 0x92, 0xcf);
    
    // User mode code segment. //
    gdt_entry_set(&gdt_entries[3], 0, 0xffffffff, 0xfa, 0xcf); 
    
    // User mode data segment. //
    gdt_entry_set(&gdt_entries[4], 0, 0xffffffff, 0xf2, 0xcf); 
    
    gdt_load();
}

//
// PS2 Keyboard driver.
//

unsigned char us_kb_scancode[128] =
{
	  0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
	'9', '0', '-', '=', '\b',                         /* Backspace. */
	'\t',                                             /* Tab. */
	'q', 'w', 'e', 'r',                               /* 19 */
	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* Enter key. */
	0,                                                /* 29 - Control. */
	'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
	'\'', '`',   0,                                   /* Left shift. */
	'\\', 'z', 'x', 'c', 'v', 'b', 'n',               /* 49 */
	'm', ',', '.', '/',   0,                          /* Right shift. */
	'*',
	0,   /* Alt */
	' ', /* Space bar */
	0,   /* Caps lock */
	0,   /* 59 - F1 key ... > */
	0,  0,   0,   0,   0,   0,   0,   0,
	0,   /* < ... F10 */
	0,   /* 69 - Num lock*/
	0,   /* Scroll Lock */
	0,   /* Home key */
	0,   /* Up Arrow */
	0,   /* Page Up */
	'-',
	0,   /* Left Arrow */
	0,
	0,   /* Right Arrow */
	'+',
	0,   /* 79 - End key*/
	0,   /* Down Arrow */
	0,   /* Page Down */
	0,   /* Insert Key */
	0,   /* Delete Key */
	0,  0,   0,
	0,   /* F11 Key */
	0,   /* F12 Key */
	0,   /* All other keys are undefined */
};

char ps2_keyboard_last_code;

int ps2_keyboard_getchar() {
	int keyboard_code=0;
	do {
		if(inb(0x60) != keyboard_code) {
			keyboard_code=inb(0x60);
			if(keyboard_code > 0 && ps2_keyboard_last_code != keyboard_code) {
				unsigned char c = us_kb_scancode[keyboard_code];
				ps2_keyboard_last_code = keyboard_code;
				if (c >= 7 && c <= 127)
					return c;
			}
		}
	} while(1);
}

//
// Kernel stuff
//

/// x86.asm functions
void a20_init();
void pmode_init();


/// The main function. 

void pmain() {
	
	puts("Welcome to NekoOS v0.1");
	while(true) {
		putchar(getchar());
	}
}

/** This functions is called from the bootup assembly. */
void kmain() {

	// Init drivers. //
	a20_init();
	vga_driver_init();
	gdt_driver_init();
	remap_irqs();
	idt_driver_init();
	outb(0x64, 0xFF);
	pmode_init();
	
	
	puts("ERROR: Entering protected mode failed!");
	while (true);
}

//=======================================
// Implement the lib.h functions.
//=======================================

int putchar(int c) {
	vga_terminal_putchar(c);
	return c;	
}

int puts(const char *str) {
	while(*str != 0)
		putchar(*str++);
	putchar('\n');
	return 1;
}

int getchar() {
	return ps2_keyboard_getchar();
	
}

size_t strlen(const char *str) {
	const char *begin = str;
	while(*str++)
		;
	return str - begin;
}

int strcmp (const char *str1, const char *str2) {
	while(*str1 && (*str1 == *str2)) {
		str1++;
		str2++;
	}
	return *(const unsigned char*)str1 - *(const unsigned char*)str2;
}

bool isalpha(const int c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool isdigit(const int c) {
	return c >= '0' && c <= '9';
}

long atol(const char *s)
{
    long num = 0;
 
    for (; s != NULL; s++)
    {
        if (*s == ' ' || *s == '\t' || isalpha(*s))
            continue;
        else
            break;
    }
    
    if (*s == NULL)
        return -1;
        
    for (; *s != NULL; s++)
    {
        if (isdigit(*s))
            num = num * 10 + (*s - '0');
        else
            break;
    }
    return num;
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


