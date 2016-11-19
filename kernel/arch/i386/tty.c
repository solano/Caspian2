#include <stddef.h>
#include <stdint.h>

#include <string.h>
#include <kernel/utils.h>
#include <kernel/tty.h>

#include "vga_text.h"

#define VGA_WIDTH 80u
#define VGA_HEIGHT 25u
static const size_t VGA_MAX_INDEX = VGA_HEIGHT*VGA_WIDTH-1; 

vga_char *term_buffer = (vga_char*) 0xB8000;

static inline uint8_t
vga_entry_color(vga_color fg, vga_color bg) {
    return fg | bg << 4;
}

static inline vga_char
vga_entry(unsigned char uc, uint8_t color) {
    vga_char entry;
    entry.color = color;
    entry.ch = (uint8_t) uc;
    return entry;
}

static inline size_t
vga_index(size_t column, size_t row) {
    return VGA_WIDTH*row + column;
}

void term_init(void) {
    term_row = 0;
    term_column = 0;
    term_color = vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

    /* VGA buffer */
    term_buffer = (vga_char*) 0xB8000;

    for(size_t y=0; y < VGA_HEIGHT; y++) {
        for(size_t x=0; x < VGA_WIDTH; x++) 
            term_putat(' ', term_color, x, y);
    }
}

/* Sets color for future writing */
void term_setcolor_i(uint8_t color) {
    term_color = color;
}

void term_setcolor(vga_color fg, vga_color bg) {
    term_setcolor_i(vga_entry_color(fg,bg));
}

/* Recolors the whole buffer */
void term_recolor_i(uint8_t color) {
    term_color = color;
    for(size_t y=0; y < VGA_HEIGHT; y++)
        for(size_t x=0; x < VGA_WIDTH; x++) 
            term_buffer[vga_index(x,y)].color = color;
}

void term_recolor(vga_color fg, vga_color bg) {
    term_recolor_i(vga_entry_color(fg,bg));
}

/* UNSAFE */
void term_putat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = vga_index(x,y);
    term_buffer[index] = vga_entry(c, color);
}

void term_newline(void) {
    if(term_row >= VGA_HEIGHT-1)
        term_shift(1);
    else
        term_row++;
    term_column = 0;
}

/* Shifts everything k lines upwards. */
void term_shift(uint8_t k) {
    for(size_t y=0; y < VGA_HEIGHT-k; y++)
        for(size_t x=0; x < VGA_WIDTH; x++) {
            size_t index = vga_index(x,y);
            term_buffer[index] = term_buffer[index+VGA_WIDTH*k];
        }
    for(size_t y=VGA_HEIGHT-k; y < VGA_HEIGHT; y++)
        for(size_t x=0; x < VGA_WIDTH; x++)
            term_putat(' ',term_color,x,y);
}

void term_putc(char c) {
    switch(c) {
        case '\n':
            term_newline();
            return;
        default:
            term_putat(c, term_color, term_column, term_row);
    }
    if (term_column >= VGA_WIDTH-1)
        term_newline();
    else
        term_column++;
}

/* UNSAFE! Could overflow buffer or write garbage */
void term_write(const char* str, size_t size) {
    for(size_t i=0; i < size; i++)
        term_putc(str[i]);
}

void term_puts(const char* str) {
    size_t len = MIN(strlen(str),
            VGA_MAX_INDEX-vga_index(term_column,term_row));
    term_write(str, len);
}
