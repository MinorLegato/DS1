#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "at91sam3x8.h"
#include "ats.h"

#define DISPLAY_WIDTH   ((i32) (30))
#define DISPLAY_HEIGHT  ((i32) (16))
#define DISPLAY_SIZE    ((i32) (DISPLAY_WIDTH * DISPLAY_HEIGHT))

#define DISPLAY_PIXEL_WIDTH  ((i32)(240))
#define DISPLAY_PIXEL_HEIGHT ((i32)(128))

#define DISPLAY_PIXEL_SIZE   ((i32)(DISPLAY_PIXEL_WIDTH * DISPLAY_PIXEL_HEIGHT))

#define DISPLAY_PIO (KEYBOARD_PIO | AT91C_PIO_PC8  | AT91C_PIO_PC12 | AT91C_PIO_PC13 \
    | AT91C_PIO_PC19 | AT91C_PIO_PC18 | AT91C_PIO_PC17 \
    | AT91C_PIO_PC16 | AT91C_PIO_PC15 | AT91C_PIO_PC14)


static u8 display_read_status(void) {
    //make databus as input
    *AT91C_PIOC_ODR = 0xFF << 2;              // PC2 - 9, PIN 34 - 41
    //Set dir as input (74chip, 1 = input)
    *AT91C_PIOC_SODR = AT91C_PIO_PC13;        // PIN 50;   
    //Clear/enable output (74chip 0 = enable)
    *AT91C_PIOC_CODR = AT91C_PIO_PC12;        // PIN 51
    //Set C/D
    *AT91C_PIOC_SODR = AT91C_PIO_PC17;        // PIN 46
    //Clear chip select display
    *AT91C_PIOC_CODR = AT91C_PIO_PC16;        // PIN 47;
    //Clear read display
    *AT91C_PIOC_CODR = AT91C_PIO_PC15;        // PIN 48
    //Make a Delay
    delay(20);
    //Read data bus and save it in temp
    u8 temp = (*AT91C_PIOC_PDSR & (0xFF << 2)) >> 2;
    //Set chip select display
    *AT91C_PIOC_SODR = AT91C_PIO_PC16;        // PIN 27
    //Set read display
    *AT91C_PIOC_SODR = AT91C_PIO_PC15;        // PIN 48;
    //Disable output (74chip)                 // Disable - 1
    *AT91C_PIOC_SODR = AT91C_PIO_PC12;        // PIN 51;
    //Set dir as output (74chip)
    *AT91C_PIOC_CODR = AT91C_PIO_PC13;        // PIN 50;
    
    return temp;
}



static __INLINE void display_write_command(u8 command) {
    // Wait until Read_Status_Display returns an OK
    while(display_read_status() == 0xFF);
    
    // Clear databus                         
    *AT91C_PIOC_CODR = (0xFF << 2);         // PC2 - 9, PIN 34 - 41
    // Set Command to databus
    *AT91C_PIOC_SODR = (command << 2);
    // Set dir as output (74chip)
    *AT91C_PIOC_CODR = AT91C_PIO_PC13;      // PIN 50
    // Enable output (74chip)
    *AT91C_PIOC_CODR = AT91C_PIO_PC12;      // PIN 51
    // Set databus as output
    *AT91C_PIOC_OER = (0xFF << 2);          // PC2 - 9, PIN 34 - 41
    // Set C/D signal High (1 = Command)
    *AT91C_PIOC_SODR = AT91C_PIO_PC17;      // PIN 46;
    // Clear chip select display
    *AT91C_PIOC_CODR = AT91C_PIO_PC16;      // PIN 47;
    // Clear write display
    *AT91C_PIOC_CODR = AT91C_PIO_PC14;      // PIN 49
    // Make a Delay
    delay(30);
    // Set chip enable display
    *AT91C_PIOC_SODR = AT91C_PIO_PC16;      // PIN 47
    // Set write display
    *AT91C_PIOC_SODR = AT91C_PIO_PC14;      // PIN 49
    // Disable output (74chip)
    *AT91C_PIOC_SODR = AT91C_PIO_PC12;      // PIN 51
    // Make databus as input 
    *AT91C_PIOC_ODR = (0xFF << 2);          // PC2 - 9, PIN 34 - 41
}

static __INLINE void display_write_data(u8 data) {
    // Wait until Read_Status_Display returns an OK
    while(display_read_status() == 0xFF);
    
    // Clear databu s
    *AT91C_PIOC_CODR = (0xFF << 2);           // PC 2 - 9, PIN 34 - 41
    // Set Data to databus
    *AT91C_PIOC_SODR = (data << 2);           
    // Set dir as output (74chip)
    *AT91C_PIOC_CODR = AT91C_PIO_PC13;        // PIN 50
    // Enable output (74chip)
    *AT91C_PIOC_CODR = AT91C_PIO_PC12;        // PIN 51
    // Set databus as output                  
    *AT91C_PIOC_OER = (0xFF << 2);            // PC 2 - 9, PIN 34 - 41
    // Clear C/D signal High (0 = Data)
    *AT91C_PIOC_CODR = AT91C_PIO_PC17;        // PIN 46
    // Clear chip select display,  CE
    *AT91C_PIOC_CODR = AT91C_PIO_PC16;        // PIN 47
    // Clear write display
    *AT91C_PIOC_CODR = AT91C_PIO_PC14;        // PIN 49
    // Make a Delay
    delay(30);
    // Set chip enable display,  CE
    *AT91C_PIOC_SODR =  AT91C_PIO_PC16;       // PIN 47
    // Set write display
    *AT91C_PIOC_SODR = AT91C_PIO_PC14;        // PIN 49
    // Disable output (74chip),  OE           // Disable - 1
    *AT91C_PIOC_SODR = AT91C_PIO_PC12;        // PIN 51
    // Make databus as input 
    *AT91C_PIOC_ODR = (0xFF << 2);            // PC 2 - 9, PIN 34 - 41
}

static __INLINE void display_set_cursor(i32 x, i32 y) {
    u16 addr = y * DISPLAY_WIDTH + x;
    
    display_write_data(addr & 0xFF);
    display_write_data((addr & 0xFF00) >> 8);
    
    display_write_command(0x24);
}

static __INLINE void display_set_pixel_cursor(i32 x, i32 y) {
    u16 addr = y * (DISPLAY_PIXEL_WIDTH / 8) + x;
    
    display_write_data(addr & 0xFF);
    display_write_data(((addr & 0xFF00) >> 8) + 0x40);
    
    display_write_command(0x24);
}

static void display_text_clear() {
    display_set_cursor(0, 0);
    
    for(i32 i = 0; i < DISPLAY_SIZE + 5; i++) { 
        display_write_data(0x00);
        display_write_command(0xC0);
    }
    
    display_set_cursor(0, 0);
}

static __INLINE void display_set_pixel(i32 x, i32 y, i32 bit) {
    display_set_pixel_cursor(x / 8, y);
    
    display_write_data(0x80 >> (x % 8));
    display_write_command(0xC4);
}



static void display_pixel_clear() {
    for (i32 y = 0; y < DISPLAY_PIXEL_HEIGHT; y++) {
        for (i32 x = 0; x < DISPLAY_PIXEL_WIDTH / 8; x++) {
            display_set_pixel_cursor(x, y);
            
            display_write_data(0x00);
            display_write_command(0xC4);
        }
    }
    
    display_set_pixel_cursor(0, 0);
}


static void display_clear() {
    display_text_clear();
    display_pixel_clear();
}


void initDisplay() {
    *AT91C_PMC_PCER = (1 << 13) | (1 << 14);
    
    *AT91C_PIOC_PER = DISPLAY_PIO;
    *AT91C_PIOC_OER = DISPLAY_PIO;
    
    // Clear Reset display
    *AT91C_PIOD_CODR = AT91C_PIO_PD0;         // PIN 25
    // Make a Delay
    delay(30000);
    // Set Reset display
    *AT91C_PIOD_SODR = AT91C_PIO_PD0;         // PIN 25
    
    display_write_data(0x00);
    display_write_data(0x00);
    display_write_command(0x40);            // Set text home address
    
    display_write_data(0x00);
    display_write_data(0x40);
    display_write_command(0x42);            // Set graphic home address
    
    display_write_data(0x1E);
    display_write_data(0x00);
    display_write_command(0x41);            // Set text area
    
    display_write_data(0x1E);
    display_write_data(0x00);
    display_write_command(0x43);            // Set graphic area
    
    display_write_command(0x80);            // text mode
    display_write_command(0x9C);            // Text on graphic on
    
    display_clear();
}

static void display_value(i32 value) {
    static const u8 data[] = {
        0x00,
        0x11, 0x12, 0x13,
        0x14, 0x15, 0x16,
        0x17, 0x18, 0x19,
        0x0A, 0x10, 0x03
    };
    
    display_write_data(data[value]);
    display_write_command(0xC4);
}

static void display_float(i32 x, i32 y, r32 value) {
    static char buffer[64];
    
    sprintf(buffer, "%.2f", value);
    
    display_set_cursor(x, y);
    
    for (i32 i = 0; buffer[i] != '\0'; i++) {
        display_write_data(buffer[i] == '.'? 0x0E : buffer[i] - ('0' - 0x10));
        display_write_command(0xC0);
    }
}

static void display_int32(i32 x, i32 y, i32 value) {
    static char buffer[64];
    
    sprintf(buffer, "%d", value);
    
    display_set_cursor(x, y);
    
    for (i32 i = 0; buffer[i] != '\0'; i++) {
        display_write_data(buffer[i] - ('0' - 0x10));
        display_write_command(0xC0);
    }
}

static void display_simple_string(i32 x, i32 y, const char* simple_str) {
    display_set_cursor(x, y);
    
    for (i32 i = 0; simple_str[i] != '\0'; i++) {
        if (simple_str[i] >= 'a' && simple_str[i] <= 'z') {
            display_write_data(simple_str[i] - ('a' - 0x41));
        } else if (simple_str[i] >= 'A' && simple_str[i] <= 'Z') {
            display_write_data(simple_str[i] - ('A' - 0x21));
        } else if (simple_str[i] == ' ') {
            display_write_data(0);
        }

        display_write_command(0xC0);
    }
}

#endif

