#ifndef __BITMAPS_H__  
#define __BITMAPS_H__

#include <string.h>
#include <stdio.h>

#include "ats.h"
#include "framebuffer.h"

// ascii bitmaps from ' ' -> '~'
static const uint64_t bitascii[128] = {
    0x000000000000000000,   // ' '
    0x000008000808080800,   // '!'
    0x000000000014140000,
    0x0000247e24247e2400,
    0x00083c42201c423c08,
    0x002152240810244a84,
    0x000c52225408141408,
    0x000000000000080800,
    0x000010080808081000,
    0x000008101010100800,
    0x000000000024182400,
    0x000008083e08080000,
    0x000004080000000000,
    0x000000007e00000000,
    0x000004000000000000,
    0x000002040810204000,
    0x000018244242241800,
    0x00001c0808080c0800,
    0x00007e041820423c00,
    0x00003c421820423c00,
    0x0000207e2428302000,
    0x00003c42403e027e00,
    0x00003c42423e023c00,
    0x000002040810207e00,
    0x00003c42423c423c00,
    0x00003c407c42423c00,
    0x000008000008000000,
    0x000004080000080000,
    0x000060180618600000,
    0x0000007e007e000000,
    0x000006186018060000,
    0x000004000c30423c00,
    0x003c02724a7a423c00,
    0x000042423c24181800,
    0x00003e42423e423e00,
    0x00003c420202423c00,
    0x00003e424242423e00,
    0x00007e02023e027e00,
    0x00000202023e027e00,
    0x00003c427a02423c00,
    0x00004242427e424200,
    0x00003e080808083e00,
    0x00003c424040407c00,
    0x000022120e0a122200,
    0x00007e020202020200,
    0x000041414955634100,
    0x00004262524a464200,
    0x00003c424242423c00,
    0x00000202023e423e00,
    0x00403c524242423c00,
    0x000042120a3a423e00,
    0x00003c423804423c00,
    0x000008080808087f00,
    0x00003c424242424200,
    0x000018242442424200,
    0x000041635549414100,
    0x000042241818244200,
    0x000008080814224100,
    0x00007e201008047e00,
    0x000038080808083800,
    0x000040201008040200,
    0x00001c101010101c00,
    0x000000000000140800,
    0x00007e000000000000,
    0x000000000000100800,
    0x00005c627c403c0000,
    0x00003a46423e020200,
    0x00003c02023c000000,
    0x00005c62427c404000,
    0x00007c027e423c0000,
    0x00000808081c081800,
    0x003840584464584000,
    0x000024241c04040400,
    0x000008080808000800,
    0x000e10101018001000,
    0x0000340c1424040400,
    0x000018080808080800,
    0x000042425a66000000,
    0x00002222263a000000,
    0x00003c42423c000000,
    0x0002023e42463a0000,
    0x0040407c42625c0000,
    0x00000202463a000000,
    0x00001c201804380000,
    0x00001808083c080000,
    0x00005c624242000000,
    0x000018244242000000,
    0x0000665a8181000000,
    0x000066182442000000,
    0x000608102844420000,
    0x00007e08107e000000,
    0x000030101818103000,
    0x000808080808080800,
    0x00000c081818080c00,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000,
    0x000000000000000000
};

#define ASCII_SIZE ((int32_t)('~' - ' '))

#define getbit(N, X, Y) (((uint64_t)(N)) & ((uint64_t)1 << (((uint64_t)(Y)) * 8 + ((uint64_t)(X)))))
#define setbit(N, X, Y) (((uint64_t)(N)) | ((uint64_t)1 << (((uint64_t)(Y)) * 8 + ((uint64_t)(X)))))

static __INLINE void render_ascii(char c, float px, float py, float x_scale, float y_scale) {
    if (c >= ' ' && c <= '~') {
        uint64_t n = bitascii[c - ' '];
        FOR_MATRIX(8, 8) {
            if (getbit(n, x, y) > 0) {
                framebuffer_draw_rect(
                    (int)(px + x * x_scale),
                    (int)(py + y * y_scale),
                    (int)(px + (x + 1) * x_scale),
                    (int)(py + (y + 1) * y_scale));
            }
        }
    }
}

static __INLINE void render_string(const char* str, float x, float y, float scale_x, float scale_y) {
    for (int32_t i = 0; str[i] != '\0'; i++)
        render_ascii(str[i], x + i * 8 * scale_x, y, scale_x, scale_y);
}

static __INLINE void render_int(int32_t n, float x, float y, float scale_x, float scale_y) {
    static char buffer[32];
    sprintf(buffer, "%d", n);
    render_string(buffer, x, y, scale_x, scale_y);
}

static __INLINE void render_float(float n, float x, float y, float scale_x, float scale_y) {
    static char buffer[32];
    sprintf(buffer, "%.1f", n);
    render_string(buffer, x, y, scale_x, scale_y);
}

static __INLINE void render_string_box(const char* str, float x, float y, float w, float h) {
    int32_t slen = strlen(str);
    float x_scale = w / ((float)slen * 8.0f);
    float y_scale = h / 8.0f;
    
    for (int32_t i = 0; i < slen; i++)
        render_ascii(str[i], x + i * 8 * x_scale, y, x_scale, y_scale);
}

static __INLINE void render_number_box(int n, float x, float y, float w, float h) {
    static char buffer[32];
    sprintf(buffer, "%d", n);
    render_string_box(buffer, x, y, w, h);
}

#endif

