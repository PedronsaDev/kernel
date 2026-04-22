#include <uart.h>
#include <serial.h>

// O linker manda o contador de programa para o binário daqui.
void kmain(void) {
    serial_init();
    serial_puts("Bem-vindo ao UFSKernel!\n");
    serial_puts("Executando em modo ARM bare-metal no QEMU.\n");
    
    uart_init();
    uart_puts("Kernel Booted. Type something and I will echo it back after a delay...\n");

    while (1) {
        for (volatile int i = 0; i < 10000000; i++); 

        char c = uart_getc(); 
        
        uart_puts("\nI received: ");
        uart_putc(c);
        uart_puts("\n");
    }
}
