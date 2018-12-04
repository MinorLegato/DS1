#ifndef __DELAY_H__
#define __DELAY_H__

#include "ats.h"

static __INLINE void delay(int value) {
    for (int i = 0; i < value; i++)
        asm("nop");
}

#endif
