#include "printf.h"
#include "uart.h"

static void print_number(unsigned int num, int base, int is_signed) {
    char buffer[32];
    int i = 0;

    // Numeros negativos
    if (is_signed && (int)num < 0) {
        uart_putc('-');
        num = (unsigned int)(-(int)num);
    }

    if (num == 0) {
        uart_putc('0');
        return;
    }

    while (num > 0) {
        int remainder = num % base;
        if (remainder < 10) {
            buffer[i++] = remainder + '0';
        } else {
            buffer[i++] = remainder - 10 + 'a';
        }
        num /= base;
    }

    while (i > 0) {
        uart_putc(buffer[--i]);
    }
}

void printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    while (*format != '\0') {
        if (*format == '%') {
            format++;

            switch (*format) {
                case 'c':{
                    char c = (char)va_arg(args, int);
                    uart_putc(c);
                    break;
                }
                case 's': {
                    char *s = va_arg(args, char *);
                    if (s) {
                        uart_puts(s);
                    } else {
                        uart_puts("(null)");
                    }
                    break;
                }
                case 'd': {
                    int n = va_arg(args, int);
                    print_number((unsigned)n, 10, 1);
                    break;
                }
                case 'u': {
                    unsigned int u = va_arg(args, unsigned int);
                    print_number(u, 10, 0);
                    break;
                }
                case 'x': {
                    unsigned int x = va_arg(args, unsigned int);
                    uart_puts("0x");
                    print_number(x, 16, 0);
                    break;
                }
                case '%': {
                    uart_putc('%');
                    break;
                }
                default: {
                    uart_putc('%');
                    uart_putc(*format);
                    break;
                }
            }
        } else {
            uart_putc(*format);
        }
        format++;
    }

    va_end(args);
}
