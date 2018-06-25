
#include <stdint.h>
#include <stdlib.h>

extern void init_drivers();
extern void rpi3_uart_putc(unsigned char c);
extern unsigned char rpi3_uart_getc();
extern void rpi3_uart_puts(const char *str);

int main() {

  init_drivers();
  rpi3_uart_puts("Drivers initialized!\n\r");

  while (1) {
    rpi3_uart_putc(rpi3_uart_getc());
  }

  return 0;
}