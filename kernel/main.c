#include <serial.h>

// O linker manda o contador de programa para o binário daqui.
void kmain(void) {
    serial_init();
    serial_puts("Bem-vindo ao grupo de memoria!\n");
	serial_puts("E aqui onde comeca o choro\n");
    serial_puts("Executando em modo ARM bare-metal no QEMU.\n");

    return;
}
