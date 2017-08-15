#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <stdint.h>

#include "ats.h"
#include "display.h"

#define FRAMEBUFFER_WIDTH   ((i32)(DISPLAY_WIDTH))
#define FRAMEBUFFER_HEIGHT  ((i32)(DISPLAY_PIXEL_HEIGHT))
#define FRAMEBUFFER_SIZE    ((i32)(FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT))

i32 __cfb = 0;  // current buffer!
u8  __framebuffer[2][FRAMEBUFFER_HEIGHT][FRAMEBUFFER_WIDTH];

static __INLINE void framebufferSetPixel(i32 x, i32 y) {
    __framebuffer[__cfb][y][x / 8] |= (0x80 >> (x % 8));
}

static __INLINE void framebufferClearPixel(i32 x, i32 y) {
    __framebuffer[__cfb][y][x / 8] &= ~(0x80 >> (x % 8));
}

static void framebufferDrawLine(i32 fx, i32 fy, i32 tx, i32 ty)
{
    r32 k = (r32)(ty - fy) / (r32)(tx - fx);
    r32 m = (r32)(fy - k * fx);
    
    for (i32 x = fx; x < tx; x++) {
        framebufferSetPixel((i32)x, (i32)(k * x + m));
    }
}

static __INLINE void framebufferDrawSnake(i32 x, i32 y)
{
    __framebuffer[__cfb][8 * y + 0][x] = 0x00;
    __framebuffer[__cfb][8 * y + 1][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 2][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 3][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 4][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 5][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 6][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 7][x] = 0x00;
}

static __INLINE void framebufferClearSnake(i32 x, i32 y)
{
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

static __INLINE void framebufferDrawApple(i32 x, i32 y)
{
    __framebuffer[__cfb][8 * y + 0][x] = 0x04;
    __framebuffer[__cfb][8 * y + 1][x] = 0x08;
    __framebuffer[__cfb][8 * y + 2][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 3][x] = 0xFF;
    __framebuffer[__cfb][8 * y + 4][x] = 0xFF;
    __framebuffer[__cfb][8 * y + 5][x] = 0xFF;
    __framebuffer[__cfb][8 * y + 6][x] = 0x7E;
    __framebuffer[__cfb][8 * y + 7][x] = 0x3C;
}

static void framebufferDrawRect(i32 fx, i32 fy, i32 tx, i32 ty)
{
    for (i32 y = fy; y < ty; y++) {
        for (i32 x = fx; x < tx; x++) {
            framebufferSetPixel(x, y);
        }
    }
}

static void framebufferClearLine(i32 fx, i32 fy, i32 tx, i32 ty)
{
    r32 k = (r32)(ty - fy) / (r32)(tx - fx);
    r32 m = (r32)(fy - k * fx);
    
    for (i32 x = fx; x < tx; x++)
        framebufferClearPixel((i32)x, (i32)(k * x + m));
}

static void framebuffeClearRect(i32 fx, i32 fy, i32 tx, i32 ty)
{
    for (i32 y = fy; y < ty; y++) {
        for (i32 x = fx; x < tx; x++)
            framebufferClearPixel(x, y);
    }
}

static void framebufferClear()
{
    for (i32 y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (i32 x = 0; x < FRAMEBUFFER_WIDTH; x++)
            __framebuffer[__cfb][y][x] = 0;
    }
}

// display changes and swap buffers!
static void framebufferDisplay()
{
    u8 current = 0;
    u8 old = 0;
    for (i32 y = 0; y < FRAMEBUFFER_HEIGHT; y++) {
        for (i32 x = 0; x < FRAMEBUFFER_WIDTH; x++) {
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

