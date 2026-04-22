#include <uart.h>

#define UART0_BASE 0x09000000U

#define UART0_CR ((volatile uint32_t *)(UART0_BASE + 0x030))

#define UART_DR ((volatile uint32_t *)(UART0_BASE + 0x000))
#define UART_FR ((volatile uint32_t *)(UART0_BASE + 0x018))

#define UART_FR_BUSY (1 << 3)
#define UART_FR_RXFE (1 << 4)
#define UART_FR_TXFF (1 << 5)

#define UART0_LCR_H ((volatile uint32_t *)(UART0_BASE + 0x02C))
#define UART_LCR_FEN (1 << 4)

#define UART0_IMSC ((volatile uint32_t *)(UART0_BASE + 0x038)) // Mascara de interrupcao
#define UART_IMSC_RXIM (1 << 4)
#define UART_IMSC_TXIM (1 << 5)

#define UART0_ICR ((volatile uint32_t *)(UART0_BASE + 0x044))
#define UART0_MIS ((volatile uint32_t *)(UART0_BASE + 0x040))

#define UART0_IBRD ((volatile uint32_t *)(UART0_BASE + 0x024))
#define UART0_FBRD ((volatile uint32_t *)(UART0_BASE + 0x028))

#define UART_BUFFER_SIZE 256

typedef struct
{
    char buffer[UART_BUFFER_SIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
} ring_buffer_t;

static ring_buffer_t rx_buffer = {.head = 0, .tail = 0};

void uart_putc(char c) {
    if (c == '\n') {
        uart_putc('\r');
    }

    while((*UART_FR) & UART_FR_TXFF){}

    *UART_DR = c;
}

void uart_puts(const char *str) {
    while (*str != '\0') {
        uart_putc(*str);
        str++;
    }
}

void uart_flush(void){
    while ((*UART_FR) & UART_FR_BUSY){}
}

static int uart_buffer_push(char c) {
    uint32_t next_head = (rx_buffer.head + 1) % UART_BUFFER_SIZE;

    if (next_head != rx_buffer.tail) {
        rx_buffer.buffer[rx_buffer.head] = c;
        rx_buffer.head = next_head;
        return 1;
    }

    return 0; // Overflow do buffer

}

static int uart_buffer_pop(char *ptr) {
    if (rx_buffer.head == rx_buffer.tail) {
        return 0;
    }

    *ptr = rx_buffer.buffer[rx_buffer.tail];
    rx_buffer.tail = (rx_buffer.tail + 1) % UART_BUFFER_SIZE;

    return 1;
}

static void uart_set_baud_rate(void) {
    *UART0_IBRD = 13;
    *UART0_FBRD = 1;
}

static void uart_enable_fifo(void) {

    *UART0_LCR_H = (0b11 << 5) | UART_LCR_FEN;
}

static void uart_enable_interrupts(void) {
    *UART0_IMSC = UART_IMSC_RXIM;
}

void uart_init(void) {

    *UART0_CR = 0;

    uart_set_baud_rate();
    uart_enable_fifo();
    uart_enable_interrupts();

    *UART0_CR = (1 << 0) | (1 << 8) | (1 << 9);
}

void uart_handler(void) {
    uint32_t status = *UART0_MIS;

    if (status & UART_IMSC_RXIM) {
        // Limpa a interrupcao
        *UART0_ICR = UART_IMSC_RXIM;

        while (!(*UART_FR & UART_FR_RXFE)) {
            char c = (char)(*UART_DR);
            uart_buffer_push(c);
        }
    }
}

char uart_getc(void) {
    char c;

    while (!uart_buffer_pop(&c)) {}

    return c;
}

