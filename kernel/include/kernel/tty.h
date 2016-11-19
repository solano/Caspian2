/* tty.h:
 * Simple terminal built on hardware text mode */

#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include <stdint.h> 

/* TODO: move this to string.h */
size_t strlen(const char* str);

size_t term_row, term_column;
uint8_t term_color;

void term_init(void);
void term_setcolor_i(uint8_t color);
//void term_setcolor(vga_color fg, vga_color bg);
void term_recolor_i(uint8_t color);
//void term_recolor(vga_color fg, vga_color bg);
void term_newline(void);
void term_shift(uint8_t shift);
void term_putat(char c, uint8_t color, size_t x, size_t y); // unsafe
void term_putc(char c);
void term_write(const char* str, size_t size); //unsafe
void term_puts(const char* str);

#endif
