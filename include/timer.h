#ifndef TIMER_H
#define TIMER_H

#include "types.h"

// frequencia das interrupcoes (Hz)
#define SYSTEM_HZ 1

// funcao publica de inicializacao
void init_timer(void);
void disable_timer(void);

#endif
