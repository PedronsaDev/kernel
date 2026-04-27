#ifndef GIC_H
#define GIC_H

typedef unsigned int uint32_t;

void gic_init(void);

uint32_t gic_acknowledge_interrupt(void);

void gic_enable_interrupt(uint32_t irq_number);

void gic_end_interrupt(uint32_t iar);

int gic_is_pending(uint32_t irq_number);

#endif // GIC_H
