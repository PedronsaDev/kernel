#include <interrupts_handler.h>
#include <serial.h>
#include <timer.h>

// (callback)
static void my_timer_callback(void) {
  serial_puts(">>> TICK DO TIMER (Via API modular)! <<<\n");

  // recarrega o timer
  uint32_t timer_ticks = 96000000;
  __asm__ volatile("mcr p15, 0, %0, c14, c2, 0" ::"r"(timer_ticks));

  // é aqui que o código fará a chamada para o
  // escalonador trocar os processos de contexto.
}

void init_timer(void) {
  uint32_t timer_ticks = 96000000;
  __asm__ volatile("mcr p15, 0, %0, c14, c2, 0" ::"r"(timer_ticks));
  uint32_t ctl = 1;
  __asm__ volatile("mcr p15, 0, %0, c14, c2, 1" ::"r"(ctl));

  // O timer registra a si mesmo no despachante do GIC
  register_interrupt_handler(TIMER_IRQ, my_timer_callback);
}
