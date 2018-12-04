#ifndef __TEMP_H__
#define __TEMP_H__

#include "system_sam3x.h"
#include "at91sam3x8.h"

#include "ats.h"
#include "delay.h"

// forward decl
int32_t systick_get_time();

#define TEMP_READY_TIME         ((int32_t)(500))
#define TEMP_COMPRESSION_OFFSET ((int32_t)(32000))

static int32_t __temp_start_time = -1;
static float __current_temp = 0.0f;
static uint16_t __current_temp_compressed = 0;

static void temp_init() {
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
    
    int32_t t_reset = systick_get_time();
    
    while((systick_get_time() - t_reset) < 520);
}

static void temp_start_mesument() {
    *AT91C_TC0_IER = 1 << 6;             // interrupt LDRBS 
    
    // create a startpuls with a Delay(25); sw_reset in TC0_CCR0.
    
    *AT91C_PIOB_OER   = AT91C_PIO_PB25;
    *AT91C_PIOB_CODR  = AT91C_PIO_PB25;
    
    delay(25);
    
    *AT91C_PIOB_ODR = AT91C_PIO_PB25;
    
    *AT91C_TC0_IDR = 1 << 6;    // disable interrupt
    *AT91C_TC0_CCR = 1 << 2;
    
    __temp_start_time = systick_get_time();
}

static __INLINE float temp_convert(int32_t rb_ra_diff) {
    return (rb_ra_diff / (5.0f * 42.0f)) - 273.15f; // divide by 42
}

static __INLINE float temp_decompress(uint16_t compressed) {
    return temp_convert(compressed + TEMP_COMPRESSION_OFFSET);
}

static void __set_current_temp() {
    // global_variable = REG_TC0_RB0 - REG_TC0_RA0 or use a flag
    int32_t rb = (*AT91C_TC0_RB); //Register B
    int32_t ra = (*AT91C_TC0_RA); //Register A 
    
    *AT91C_TC0_SR;  // status register
    
    __current_temp_compressed = (rb - ra) - TEMP_COMPRESSION_OFFSET;
    __current_temp = temp_convert(rb - ra);
}

static __INLINE int32_t temp_ready() {
    if ((__temp_start_time != -1) && (__temp_start_time + TEMP_READY_TIME) < systick_get_time()) {
        __temp_start_time = -1;
        __set_current_temp();
        return 1;
    }
    return 0;
}

static __INLINE uint16_t temp_get_compressed() {
    return __current_temp_compressed;
}

static __INLINE float temp_get_current() {
    return __current_temp;
}

#endif

