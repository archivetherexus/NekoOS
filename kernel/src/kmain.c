#include "kdrivers/vga.h"
#include "kdrivers/ps2.h"

void kmain()
{
	
	terminal_write_string("Hello World from Ring 0!");

	while (1) {
		char in = ps2_get_char();
		char i[2] = {in, 0};
		terminal_write_string(i);
	}
}
