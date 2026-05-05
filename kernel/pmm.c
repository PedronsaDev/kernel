// Gerencia o array de bits (Bitmap)
#include "pmm.h"

 extern uint8_t __end[]; // Símbolo vindo do linker script, representa o final do kernel

uint32_t *bitmap;

void pmm_init() {
    bitmap = (uint32_t *) __end; //tem que alinhar aqui

    uint32_t num_pages  = RAM_SIZE / PAGE_SIZE;
    uint32_t bitmap_size = num_pages / 32;
 
    for (uint32_t i = 0; i < bitmap_size; i++) {
        bitmap[i] = 0xFFFFFFFF;
    }

    uintptr_t free_mem_start = (uintptr_t) __end + bitmap_size * sizeof(uint32_t);

    for(uintptr_t addr = free_mem_start; addr < RAM_END; addr += PAGE_SIZE) {
        pmm_free_page(addr);
    }

}


void pmm_free_page(uintptr_t addr) {
    uint32_t page = addr / PAGE_SIZE;
    uint32_t idx = page / 32;
    uint32_t bit = page % 32;

    uint32_t mask = ~(1 << bit);
    bitmap[idx] &= mask;
}

void pmm_alloc_page_at(uintptr_t addr) {
    uint32_t page = addr / PAGE_SIZE;
    uint32_t idx = page / 32;
    uint32_t bit = page % 32;
    
    uint32_t mask = (1 << bit);
    bitmap[idx] |= mask; 
}