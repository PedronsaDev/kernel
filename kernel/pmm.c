// Gerencia o array de bits (Bitmap).
#include "mem_layout.h"
#include "pmm.h"

extern uint8_t __end[]; // Símbolo vindo do linker script, representa o final do kernel

uint32_t *bitmap;

void pmm_init() {
    bitmap = (uint32_t *) __end;

    uint32_t num_frames  = RAM_SIZE / PAGE_SIZE;
    uint32_t bitmap_size = num_frames / 8;

    for (uint32_t i = 0; i < (bitmap_size / 4); i++) {
        bitmap[i] = 0xFFFFFFFF;
    }

    uintptr_t free_mem_start = (uintptr_t) __end + bitmap_size;

    for(uintptr_t addr = free_mem_start; addr < RAM_END; addr += PAGE_SIZE) {
        pmm_free_frame(addr);
    }

}


void pmm_free_frame(uintptr_t addr) {
    uint32_t frame = addr / PAGE_SIZE;
    uint32_t idx = frame / 32;
    uint32_t bit = frame % 32;

    bitmap[idx] &= ~(1 << bit);
}

void pmm_alloc_frame_at(uintptr_t addr) {
    uint32_t frame = addr / PAGE_SIZE;
    uint32_t idx = frame / 32;
    uint32_t bit = frame % 32;

    bitmap[idx] |= (1 << bit); 
}