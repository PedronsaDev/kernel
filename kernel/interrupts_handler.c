#include <interrupts_handler.h>

static isr_t interrupt_handlers[MAX_INTERRUPTS] = {0};

void register_interrupt_handler(uint32_t irq, isr_t handler) {
  if (irq < MAX_INTERRUPTS) {
    interrupt_handlers[irq] = handler;
  }
}

void init_gic(void) {
  GICD_CTLR = 1;
  GICC_CTLR = 1;
  GICC_PMR = 0xFF;
  GICD_ISENABLER0 = (1 << TIMER_IRQ);
}

void enable_cpu_interrupts(void) { __asm__ volatile("cpsie i"); }

// Antiga timer_interrupt_c() que agora é genérica
void irq_dispatcher_c(void) {
  uint32_t irq_id = GICC_IAR & 0x3FF;

  if (irq_id < MAX_INTERRUPTS && interrupt_handlers[irq_id] != 0) {
    interrupt_handlers[irq_id](); // Aciona o timer.c (ou outro módulo)
  }

  GICC_EOIR = irq_id;
}
