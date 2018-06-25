#pragma once
#ifndef RPI3_UART_H
#define RPI3_UART_H

void rpi3_uart_init();
void rpi3_uart_putc(unsigned char c);
unsigned char rpi3_uart_getc();
void rpi3_uart_puts(const char* str);

#endif