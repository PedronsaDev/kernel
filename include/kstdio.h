#ifndef KSTDIO_H
#define KSTDIO_H

#include <stdarg.h>

// Inicializacao
void ksdtio_init(void);

// Output
void kprintf(const char *format, ...);
void kputs(const char *str);
void kputc(char c);

// Input
char kgetc(void);

// Utilidade
int kstdio_has_data(void);

#endif // KSTDIO_H
