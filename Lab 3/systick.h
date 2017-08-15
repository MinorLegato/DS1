#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "ats.h"

void tempStartMesurment();
i32 tempReady();

i32 __systick_counter = 0;

void SysTick_Handler(void) {
    __systick_counter++;

    if (tempReady()) {
        tempStartMesurment();
    }
}

static void initSystick() {
    *AT91C_PMC_PCER = (1 << 3);
    SysTick_Config(84000);
}

static i32 systickGetTime() {
    return __systick_counter;
}

#endif 

