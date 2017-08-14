#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "ats.h"
#include "temp.h"

i32 __systick_counter = 0;

void SysTick_Handler(void)
{
    __systick_counter++;

    if (tempReady()) {
        tempStartMesurment();
        //printf("%f\n", tempGetCurrent());
    }
}

static void initSystick()
{
    *AT91C_PMC_PCER = (1 << 3);
    SysTick_Config(84000);
}

static __INLINE i32 systickGetTime()
{
    return __systick_counter;
}

#endif 

