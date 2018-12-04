#ifndef __LIGHT_SENSOR_H__
#define __LIGHT_SENSOR_H__


#include "at91sam3x8.h"

static void light_sensor_init() {
    *AT91C_PMC_PCER = (1 << 11);                    // init PMC PIOA
    *AT91C_PMC_PCER1 = (1 << 5);                    // enable PMC ADC
    *AT91C_ADCC_MR = *AT91C_ADCC_MR | (1 << 9);     // set prescaler set 2 - (bit 8 = 1)
}

static float light_sensor_read() {
    *AT91C_ADCC_CHER = (1 << 1);        // select channel
    *AT91C_ADCC_CR   = (1 << 1);        // start
    
    // RXBUFF | ENDRX | DRDY = 0x19000000   (while data not ready)
    while(!((*AT91C_ADCC_SR & 0x19000000) == 0x19000000));
    
    return *AT91C_ADCC_LCDR;    // Last Converted Data
}

#endif
