#include <serial.h>

#include "kstdio.h"
#include "gic.h"
#include "shell.h"

// O linker manda o contador de programa para o binário daqui.
void kmain(void) {
    serial_init();
    serial_puts("Bem-vindo ao UFSKernel!\n");
    serial_puts("Executando em modo ARM bare-metal no QEMU.\n");

    ksdtio_init();
    
    // Habilita as interrupcoes do ID 33(UART)
    // e coloca o target na CPU 0
    gic_init();
    gic_enable_interrupt(33);

    shell_run();
}
