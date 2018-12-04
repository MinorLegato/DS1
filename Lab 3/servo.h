#ifndef __SERVO_H__
#define __SERVO_H__

#include "at91sam3x8.h"
#include "ats.h"

#define ONE_DEGREE ((float)((2.3 - 0.7) / 180))  // 1 degree -> (max - min) / 180
#define MS_PULSE   ((int32_t)(2625))             // 1 ms -> (52500 / 20)

void servo_turn(int32_t degree) {
    // write value to pwm_cdty, which means set duty cycle to 1ms, cprd / 20 = 1ms = 2625
    if(degree >= 0 && degree <= 180) {
        *AT91C_PWMC_CH1_CDTYR = (int32_t)(((ONE_DEGREE * degree) + 0.7) * MS_PULSE);
    }
}

static void servo_init() {
    *AT91C_PMC_PCER  = (1 << 12);       // init PMC PIOB
    *AT91C_PMC_PCER1 = (1 << 4);        // init PMC PWM 36

    *AT91C_PIOB_PDR  = AT91C_PIO_PB17;  // disable pin
    *AT91C_PIOB_ABMR = AT91C_PIO_PB17;  // <- ABSR

    *AT91C_PWMC_ENA  = (1 << 1);        // enable chanel id 1

    *AT91C_PWMC_CH1_CMR = 0x5 ;         //set prescaler set 2^5 = 32

    // set period: 20 ms; --- 32 * CPRD / 84000 = 20 <=> CPRD = 52500   (s991)
    *AT91C_PWMC_CH1_CPRDR = 52500;
   
    servo_turn(0);
}

#endif
