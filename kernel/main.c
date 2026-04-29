#include <serial.h>

#include "kstdio.h"

// O linker manda o contador de programa para o binário daqui.
void kmain(void) {
    serial_init();
    serial_puts("Bem-vindo ao UFSKernel!\n");
    serial_puts("Executando em modo ARM bare-metal no QEMU.\n");

    uart_init();
    gic_init();

    // Habilita as interrupcoes do ID 33(UART)
    // e coloca o target na CPU 0
    gic_enable_interrupt(33);

    printf("+========PRINTF========+\n");
    printf("Teste printf sem parametros\n");
    printf("Teste String: %s\n", "Hello World");
    printf("Teste Char: %c\n", 'A');
    printf("Teste Decimal(int): %d and %d\n", 67, -451);
    printf("Teste Hex: %x\n", 0x09000000);
    printf("+======================+\n");
    
    printf("UART e GIC rodando em modo manual.\n");
    printf("Digite alguma coisa. O GIC deve detectar...\n");
    // Precisa linkar com a vector table
    while (1) {
        if (gic_is_pending(33)) {
            // uart_puts("GIC detectou IRQ 33 pendente.\n");

            // Simulando a vector table
            uint32_t iar = gic_acknowledge_interrupt();
            if ((iar & 0x3FF) == 33) {
                uart_handler();
            }

            gic_end_interrupt(iar);

            // Verifica se conseguimos escrever no buffer
            char c = uart_getc();
            printf("Caracter lido do buffer: %c", c);
            uart_puts("\n");
        }

        // Adiciona um pouco de delay
        for (volatile int i = 0; i < 500000; i++);
    }
}
