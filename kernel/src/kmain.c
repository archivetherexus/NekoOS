#include "kdrivers/vga.h"

void kmain()
{
	vga_clear_screen();
	terminal_write_string("Hello World from ring 0!");
}
