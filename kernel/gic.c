#include "gic.h"

#define GIC_DIST_BASE  0x08000000U  // Distribuidor GIC
#define GIC_CPU_BASE   0x08010000U  // interface da CPU

// Registradores do GIC
#define GICD_CTLR   ((volatile uint32_t *)(GIC_DIST_BASE + 0x000))
#define GICD_ISENABLER(n)   ((volatile uint32_t *)(GIC_DIST_BASE + 0x100 + (n) * 4))
#define GICD_ITARGETSR(n)   ((volatile uint32_t *)(GIC_DIST_BASE + 0x800 + (n) * 4))
#define GICD_IPRIORITYR(n)  ((volatile uint32_t *)(GIC_DIST_BASE + 0x400 + (n) * 4))

// Registradores da interface de CPU
#define GICC_CTLR   ((volatile uint32_t *)(GIC_CPU_BASE + 0x000))
#define GICC_PMR    ((volatile uint32_t *)(GIC_CPU_BASE + 0x004))
#define GICC_IAR    ((volatile uint32_t *)(GIC_CPU_BASE + 0x00C))
#define GICC_EOIR   ((volatile uint32_t *)(GIC_CPU_BASE + 0x010))

void gic_init(void) {
    // Ativa o destribuidor
    *GICD_CTLR = 1;

    // Coloca a prioridade pra receber todas as interrupcoes
    *GICC_PMR = 0xFF;

    // Ativa a interface da CPU
    *GICC_CTLR = 1;
}

uint32_t gic_acknowledge_interrupt(void) { 
    return *GICC_IAR;
}

void gic_enable_interrupt(uint32_t irq_number) {
    uint32_t reg_idx = irq_number / 32;
    uint32_t bit_idx = irq_number % 32;
    GICD_ISENABLER(reg_idx)[0] = (1 << bit_idx);
    
    // Direciona o target para a CPU 0
    uint32_t target_idx = irq_number / 4;
    uint32_t target_shift = (irq_number % 4) * 8;

    // Limpa o target anterior e seleciona a CPU 0
    uint32_t val = GICD_ITARGETSR(target_idx)[0];
    val &= ~(0xFF << target_shift);
    val |= (0x01 << target_shift);
    GICD_ITARGETSR(target_idx)[0] = val;
}

void gic_end_interrupt(uint32_t iar) {
    *GICC_EOIR = iar;
}

int gic_is_pending (uint32_t irq_number) {
    uint32_t reg_idx = irq_number / 32;
    uint32_t bit_idx = irq_number % 32;
  
    // GIC_ISPENDR ta com offset de 0x200
    volatile uint32_t *pending_reg = (volatile uint32_t *)(GIC_DIST_BASE + 0x200 + (reg_idx * 4));
    return (*pending_reg & (1 << bit_idx));
}
