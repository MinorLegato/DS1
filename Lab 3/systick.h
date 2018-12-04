#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "ats.h"

uint32_t is_logging = 0;

// forward decl
void temp_start_mesument();
int temp_ready();
void log_current_temp();

int32_t __systick_counter = 0;

void SysTick_Handler(void) {
    __systick_counter++;

    if (temp_ready()) {
        if (is_logging) { log_current_temp(); }

        temp_start_mesument();
    }
}

static void systick_init() {
    *AT91C_PMC_PCER = (1 << 3);
    SysTick_Config(84000);
}

static int systick_get_time() {
    return __systick_counter;
}

#endif 

