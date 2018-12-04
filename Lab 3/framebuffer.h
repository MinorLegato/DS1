#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <stdint.h>

#include "ats.h"
#include "display.h"

#define FRAMEBUFFER_WIDTH   ((int32_t)(DISPLAY_WIDTH))
#define FRAMEBUFFER_HEIGHT  ((int32_t)(DISPLAY_PIXEL_HEIGHT))
#define FRAMEBUFFER_SIZE    ((int32_t)(FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT))

int32_t __cfb = 0;  // current buffer!
uint8_t  __framebuffer[2][FRAMEBUFFER_HEIGHT][FRAMEBUFFER_WIDTH];

static __INLINE void framebuffer_set_pixel(int32_t x, int32_t y) {
    __framebuffer[__cfb][y][x / 8] |= (0x80 >> (x % 8));
}

static __INLINE void framebuffer_clear_pixel(int32_t x, int32_t y) {
    __framebuffer[__cfb][y][x / 8] &= ~(0x80 >> (x % 8));
}

static void framebuffer_draw_line(int32_t fx, int32_t fy, int32_t tx, int32_t ty) {
    float k = (float)(ty - fy) / (float)(tx - fx);
    float m = (float)(fy - k * fx);
    
    for (int32_t x = fx; x < tx; x++) {
        framebuffer_set_pixel((int32_t)x, (int32_t)(k * x + m));
    }
}

static __INLINE void framebuffer_draw_snake(int32_t x, int32_t y) {
    __framebuffer[__cfb][8 * y + 0][x] = 0x00;
    __framebuffer[__cfb][8 * y + 1][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 2][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 3][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 4][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 5][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 6][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 7][x] = 0x00;
}

static __INLINE void framebuffer_clear_snake(int32_t x, int32_t y) {
    __framebuffer[__cfb][8 * y + 0][x] = 0x00;
    __framebuffer[__cfb][8 * y + 1][x] = 0x00;
    __framebuffer[__cfb][8 * y + 2][x] = 0x00;
    __framebuffer[__cfb][8 * y + 3][x] = 0x00;
    __framebuffer[__cfb][8 * y + 4][x] = 0x00;
    __framebuffer[__cfb][8 * y + 5][x] = 0x00;
    __framebuffer[__cfb][8 * y + 6][x] = 0x00;
    __framebuffer[__cfb][8 * y + 7][x] = 0x00;
}

//  "      1  
//  "     1   
//  "  111111 
//  " 11111111
//  " 11111111
//  " 11111111
//  "  111111 
//  "   1111  

static __INLINE void framebuffer_draw_apple(int32_t x, int32_t y) {
    __framebuffer[__cfb][8 * y + 0][x] = 0x04;
    __framebuffer[__cfb][8 * y + 1][x] = 0x08;
    __framebuffer[__cfb][8 * y + 2][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 3][x] = 0xFF;
    __framebuffer[__cfb][8 * y + 4][x] = 0xFF;
    __framebuffer[__cfb][8 * y + 5][x] = 0xFF;
    __framebuffer[__cfb][8 * y + 6][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 7][x] = 0x3C;
}

static void framebuffer_draw_rect(int32_t fx, int32_t fy, int32_t tx, int32_t ty) {
    for (int32_t y = fy; y < ty; y++) {
        for (int32_t x = fx; x < tx; x++) {
            framebuffer_set_pixel(x, y);
        }
    }
}

static void framebuffer_clear_line(int32_t fx, int32_t fy, int32_t tx, int32_t ty) {
    float k = (float)(ty - fy) / (float)(tx - fx);
    float m = (float)(fy - k * fx);
    
    for (int32_t x = fx; x < tx; x++)
        framebuffer_clear_pixel((int32_t)x, (int32_t)(k * x + m));
}

static void framebuffeClearRect(int32_t fx, int32_t fy, int32_t tx, int32_t ty) {
    for (int32_t y = fy; y < ty; y++) {
        for (int32_t x = fx; x < tx; x++)
            framebuffer_clear_pixel(x, y);
    }
}

static void framebuffer_clear() {
    for (int32_t y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (int32_t x = 0; x < FRAMEBUFFER_WIDTH; x++)
            __framebuffer[__cfb][y][x] = 0;
    }
}

static void framebuffer_display_no_swap() {
    uint8_t current = 0;
    for (int32_t y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (int32_t x = 0; x < FRAMEBUFFER_WIDTH; x++) {
            current = __framebuffer[__cfb][y][x];
            display_set_pixel_cursor(x, y);
            display_write_data(current);
            display_write_command(0xC4);
        }
    }
}

// display changes and swap buffers!
static void framebuffer_display() {
    uint8_t current = 0;
    uint8_t old = 0;
    for (int32_t y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (int32_t x = 0; x < FRAMEBUFFER_WIDTH; x++) {
            current = __framebuffer[__cfb][y][x];
            old     = __framebuffer[!__cfb][y][x];
            if (current != old) {
                display_set_pixel_cursor(x, y);
                display_write_data(current);
                display_write_command(0xC4);
            }
        }
    }
    __cfb = !__cfb;
    memset(__framebuffer[__cfb], 0, sizeof __framebuffer[0]);
}

#endif

