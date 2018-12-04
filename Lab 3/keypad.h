#ifndef __KEYPAD_H__
#define __KEYPAD_H__


#include "at91sam3x8.h"
#include "ats.h"

#define KEYBOARD_PIO (AT91C_PIO_PC2 | AT91C_PIO_PC3 | AT91C_PIO_PC4 | AT91C_PIO_PC5 | \
                      AT91C_PIO_PC7 | AT91C_PIO_PC8 | AT91C_PIO_PC9)

#define KEYBOARD_ROW (AT91C_PIO_PC2 | AT91C_PIO_PC3 | AT91C_PIO_PC4 | AT91C_PIO_PC5)
#define KEYBOARD_COL (AT91C_PIO_PC7 | AT91C_PIO_PC8 | AT91C_PIO_PC9)

static void keypad_init() {
    *AT91C_PMC_PCER = (1 << 13);
    
    *AT91C_PIOC_PER = KEYBOARD_PIO;
    *AT91C_PIOC_OER = KEYBOARD_PIO;
    
    *AT91C_PIOD_PER  = AT91C_PIO_PD0 | AT91C_PIO_PD2;
    *AT91C_PIOD_OER  = AT91C_PIO_PD0 | AT91C_PIO_PD2;
    *AT91C_PIOD_SODR = AT91C_PIO_PD0 | AT91C_PIO_PD2;
}

static __INLINE int32_t __read_row() { 
    return ((*AT91C_PIOC_PDSR & KEYBOARD_ROW) >> 2);
}

static __INLINE void __set_colunm(int32_t col) {
    *AT91C_PIOC_SODR = KEYBOARD_COL;    // set all columns as 1
    *AT91C_PIOC_CODR = 1 << (col + 7);  // clear the column 'col'
}

static int keypad_read() {
    int32_t value = 0;
    // clear OE KEY BUS (Active Low)   
    *AT91C_PIOD_CODR = AT91C_PIO_PD2; // PIN 27
    // make all column pin as output 
    *AT91C_PIOC_OER = KEYBOARD_COL;

    // Loop Column
    for (int32_t n = 0; n < 3; n++) {
        __set_colunm((n + 1) % 3);
        
        for (int32_t i = 0; i < 4; i++) {
            if((~__read_row() & (1 << i)) == (1 << i))
                value = ((i + 1) % 4) * 3 + n + 1; 
        }
    }
    
    *AT91C_PIOC_ODR = KEYBOARD_COL; // make all column pin as input 
    
    return value;
}

static int keypad_wait() {
    int32_t value = 0;
    do { value = keypad_read(); } while(value == 0);
    return value;
}

#endif
