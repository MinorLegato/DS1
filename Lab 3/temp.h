#ifndef __TEMP_H__
#define __TEMP_H__

#include "system_sam3x.h"
#include "at91sam3x8.h"

#include "ats.h"
#include "systick.h"
#include "delay.h"

#define TEMP_READY_TIME ((i32)(500))
#define TEMP_COMPRESSION_OFFSET ((i32)(32000))

static i32 __temp_start_time = -1;
static r32 __current_temp = 0.0f;
static u16 __current_temp_compressed = 0;

static void initTemp()
{
    *AT91C_PMC_PCER = (1 << 12) | (1 << 27);    // init TC0
    
    // Enable counter and make a sw_reset in TC0_CCR0
    *AT91C_TC0_CCR = 0x5;           // SWTRG | CLKEN
    
    *AT91C_TC0_CMR = 0x60000;       // TIOA (A - falling - 10b), (B - rising - 01b) and TCCLK1
    
    //Enable the pin
    *AT91C_PIOB_PER = AT91C_PIO_PB25;
    *AT91C_PIOB_ODR = AT91C_PIO_PB25;
    
    NVIC_ClearPendingIRQ(TC0_IRQn);
    NVIC_SetPriority(TC0_IRQn, 1);
    NVIC_EnableIRQ(TC0_IRQn);
    
    i32 t_reset = systickGetTime();
    
    while((systickGetTime() - t_reset) < 520);
}

static void tempStartMesurment()
{
    *AT91C_TC0_IER = 1 << 6;             // interrupt LDRBS 
    
    // create a startpuls with a Delay(25); sw_reset in TC0_CCR0.
    
    *AT91C_PIOB_OER   = AT91C_PIO_PB25;
    *AT91C_PIOB_CODR  = AT91C_PIO_PB25;
    
    delay(25);
    
    *AT91C_PIOB_ODR = AT91C_PIO_PB25;
    
    *AT91C_TC0_IDR = 1 << 6;    // disable interrupt
    *AT91C_TC0_CCR = 1 << 2;
    
    __temp_start_time = systickGetTime();
}

static __INLINE r32 tempConvert(i32 rb_ra_diff)
{
    return (rb_ra_diff / (5.0f * 42.0f)) - 273.15f; // divide by 42
}

static __INLINE r32 tempDecompress(u16 compressed)
{
    return tempConvert(compressed + TEMP_COMPRESSION_OFFSET);
}

static void __set_current_temp()
{
    // global_variable = REG_TC0_RB0 - REG_TC0_RA0 or use a flag
    i32 rb = (*AT91C_TC0_RB); //Register B
    i32 ra = (*AT91C_TC0_RA); //Register A 
    
    *AT91C_TC0_SR;  // status register
    
    __current_temp_compressed = (rb - ra) - TEMP_COMPRESSION_OFFSET;
    __current_temp = tempConvert(rb - ra);
}

static __INLINE i32 tempReady()
{
    if ((__temp_start_time != -1) && (__temp_start_time + TEMP_READY_TIME) < systickGetTime()) {
        __temp_start_time = -1;
        __set_current_temp();
        return 1;
    }
    return 0;
}

static __INLINE u16 tempGetCompressed()
{
    return __current_temp_compressed;
}

static __INLINE r32 tempGetCurrent()
{
    return __current_temp;
}

#endif
