#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#include <kernel/tty.h>
#include <string.h>

#include <kernel/kio.h>

/* Integer to string in base-<radix> */
char* _itoa(uint32_t n, const uint8_t radix, const char* algarisms) {
/*    if(radix >= 36)
        abort(); // TODO */

    /* Repeated division gives us the consecutive caracters
     * from right to left. At the end, we reverse the string */
    char aux[32]; // FIXME: this assumes numbers no longer than 32 bits. Should use malloc
    int i, j;
    i = 0;
    do {
        j = n % radix;
        aux[i] = algarisms[j];
        n -= j;
        n /= radix;
        i++;
    } while (n);
    aux[i] = '\0';

    return reverse(aux);
}

inline char* itoa(uint32_t n, const uint8_t radix) {
    return _itoa(n, radix, "0123456789abcdefghijklmnopqrstuvwxyz");
}
inline char* itoA(uint32_t n, const uint8_t radix) {
    return _itoa(n, radix, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
}

/* Aliases. Meant to isolate tty.h stuff from kio.h stuff */
inline void kputc(const char c) {
    term_putc(c);
}
inline void kprint(const char* str) {
    term_puts(str);
}
inline void kwrite(const char* str, size_t n) {
    term_write(str, n);
}

/* FIXME: this function assumes 32-bit architecture */
void kprintf(const char* str, ...) {
    va_list ap;
    va_start(ap, str);

    unsigned int i = 0;   // index
    unsigned int k = 0;   // minimum number of digits to print
    size_t j = 0;         // auxiliary buffer
    uint32_t n = 0;       // receives unsigned integer arguments
    char *aux;            // buffer
    bool esc = false;     // whether we're evaluating an escape expression

    char c;
    while ((c = str[i++])) {
        if (!esc)
        switch (c) {
            case '%':
                esc = true;
                break;
            default:
                kputc(c);
                break;
        }

        else
        switch(c) {
            case '%':
                kputc('%');
                break;

            case '0'...'9':
                k *= 10;
                k += c-'0';
                break;

            case 'u':
            case 'x':
            case 'X':
                n = va_arg(ap, uint32_t);
                if (!n) break; // TODO: abort? compile error?

                switch(c) {
                    case 'u':
                        aux = itoa(n, 10);
                        break;
                    case 'x':
                        aux = itoa(n, 16);
                        break;
                    case 'X':
                        aux = itoA(n, 16);
                        break;
                }

                /* Print zeroes to satisfy digit condition */
                j = strlen(aux);
                if(j < k)
                    for(k-=j; k>0; k--)
                        kputc('0');
                kprint(aux);
                k = 0;
                esc = false;
                break;

            /* In the case of a string, k acts as a *maximum* number
             * of characters to display */
            case 's':
                aux = va_arg(ap, char*);
                j = strlen(aux);
                if (j>k && k!=0)
                    kwrite(aux, k);
                else
                    kprint(aux);
                k = 0;
                esc = false;
                break;

            /* TODO: support for signed decimals, floats,
             * and everything else */
            default:
                esc = false;
                break; // TODO: abort? compile error?
        }
    }
    va_end(ap);
}

void ktest(void) {
    kprint("TEST\n");
    }
