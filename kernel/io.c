#include <io.h>

typedef unsigned int uint32_t;

#define UART0_BASE 0x09000000

#define UART0_DR ((volatile uint32_t *)(UART0_BASE + 0x000))
#define UART0_FR ((volatile uint32_t *)(UART0_BASE + 0x018))

#define UART_FR_RXFE (1 << 4)
#define UART_FR_TXFF (1 << 5)

char io_getc(void){
    while((*UART0_FR) & UART_FR_RXFE){/* Espera ocupada*/}
    
    return (char)(*UART0_DR);
}

void io_putc(char c){
    while((*UART0_FR) & UART_FR_TXFF){/* Espera ocupada*/}

    *UART0_DR = c;
}
