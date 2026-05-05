// Physical Memory Manager - Protótipos das funções

#ifndef PMM_H
#define PMM_H

#include <stdint.h>

#define RAM_START        0x40000000  // Início da RAM no QEMU

/* Tamanho da RAM - Vamos definir 128MB por padrão */
#define RAM_SIZE         (128 * 1024 * 1024) 

#define RAM_END          (RAM_START + RAM_SIZE)

/* Endereço da UART (que você já usa no serial.c) */
#define UART0_ADDRESS    0x09000000

/* Tamanho de cada página/bloco de memória (4 KB) */
#define PAGE_SIZE        4096 

// Inicializa o gerenciador a partir do end
void pmm_init();

// Aloca um bloco de 4KB e retorna seu endereço físico
void* pmm_alloc_block();

// Libera um bloco previamente alocado
void pmm_free_block(void* addr);

#endif

