/* vga_text.h:
 * Used by tty.c */

#include <stdint.h> 

typedef enum vga_color {
    VGA_COLOR_BLACK=0,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_BROWN,
    VGA_COLOR_LIGHT_GREY,
    VGA_COLOR_DARK_GREY,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_LIGHT_MAGENTA,
    VGA_COLOR_LIGHT_BROWN,
    VGA_COLOR_WHITE
} vga_color;

/* Why does this work, and not the other way around?
 * Probably because x86 processors are little-endian.
 * (Remember: color code comes BEFORE character) */
typedef struct {
    uint8_t ch;
    uint8_t color;
} vga_char;
