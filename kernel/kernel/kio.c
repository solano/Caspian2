#include <stdint.h>
#include <stdarg.h>

#include <kernel/tty.h>
#include <string.h>

#include <kernel/kio.h>

/* Integer to string in base-<radix> */
char* itoa(uint32_t n, const uint8_t radix) {
/*    if(radix >= 36)
        abort(); // TODO */

    /* Repeated division gives us the consecutive caracters
     * from right to left. At the end, we reverse the string */
    char aux[32]; // FIXME: this assumes numbers no longer than 32 bits
    int i, j;
    i = 0;
    do {
        j = n % radix;
        aux[i] = "0123456789abcdefghijklmnopqrstuvwxyz"[j];
        n -= j;
        n /= radix;
        i++;
    } while (n);
    aux[i] = '\0';

    return reverse(aux);
}

void kprint(const char* str) {
    term_puts(str);
}

void kprintf(const char* str, ...) {
    va_list ap;
    va_start(ap, str);

    int i = 0;
    uint32_t n = 0;
    char *aux1, aux2[32]; // FIXME: this assumes 32-bit
    char c;
    while ((c = str[i++])) {
        switch (c) {
            case '%':
                switch((c = str[i++])) {
                    case '%':
                        term_putc('%');
                        break;
                    case 'u':
                        n = va_arg(ap, uint32_t);
                        if (!n) break; // TODO: abort? compile error?
                        aux1 = itoa(n, 10);
                        memcpy(aux2, aux1, strlen(aux1));
                        kprint(aux1);
                        break;
                    case 'x':
                        n = va_arg(ap, uint32_t);
                        if (!n) break; // same as above
                        aux1 = itoa(n, 16);
                        memcpy(aux2, aux1, strlen(aux1));
                        kprint(aux1);
                        break;
                    /* TODO: support for signed decimals, strings,
                     * and everything else */
                    default:
                        break; // TODO: abort? compile error?
                }
                break;
            default:
                term_putc(c);
                continue;
        }
    }
    va_end(ap);
}
