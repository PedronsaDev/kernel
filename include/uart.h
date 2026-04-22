#ifndef UART_H
#define UART_H

typedef unsigned int uint32_t;

void uart_putc(char c);
void uart_puts(const char *str);
void uart_flush(void);

void uart_init(void);
void uart_handler(void);
char uart_getc(void);

#endif // UART_H
