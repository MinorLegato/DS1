#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "ats.h"

i32 is_logging = 0;

// forward decl
static void temp_start_mesument();
static int  temp_ready();
static void log_current_temp();

static int __systick_counter = 0;

void SysTick_Handler(void) {
    __systick_counter++;

    if (temp_ready()) {
        if (is_logging) { log_current_temp(); }

        temp_start_mesument();
    }
}

static static void systick_init() {
    *AT91C_PMC_PCER = (1 << 3);
    SysTick_Config(84000);
}

static int systick_get_time() {
    return __systick_counter;
}

#endif 

