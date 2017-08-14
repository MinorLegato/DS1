#ifndef __DELAY_H__
#define __DELAY_H__

#include "ats.h"


static __INLINE void delay(i32 value)
{
    for (i32 i = 0; i < value; i++)
        asm("nop");
}



#endif
