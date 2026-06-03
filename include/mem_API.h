#ifndef MEM_API_H
#define MEM_API_H

#include "pmm.h"
#include "VMM.h"

uintptr_t kmalloc(uint32_t size); //Função equivalente ao malloc
uint8_t kfree(uintptr_t addr); //Função equivalente ao free

#endif