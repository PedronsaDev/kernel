#include <serial.h>
#include "uart.h"
#include "gic.h"

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

    uart_puts("UART e GIC rodando em modo manual.\n");
    uart_puts("Digite alguma coisa. O GIC deve detectar...\n");

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
            uart_puts("Caracter lido do buffer: ");
            uart_putc(c);
            uart_puts("\n");
        }

        // Adiciona um pouco de delay
        for (volatile int i = 0; i < 500000; i++);
    }
}
