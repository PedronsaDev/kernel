#ifndef IO_H
#define IO_H

#define IO_BUFFER_SIZE 256

typedef unsigned int uint32_t;

typedef struct
{
    char buffer[IO_BUFFER_SIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
} ring_buffer_t;

char io_getc(void);
void io_putc(char c);
void io_puts(const char *str);
void io_flush(void);

#endif // IO_H
