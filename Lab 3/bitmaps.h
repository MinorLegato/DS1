#ifndef __BITMAPS_H__  
#define __BITMAPS_H__

#include <string.h>
#include <stdio.h>

#include "ats.h"
#include "framebuffer.h"

static u64 bitascii[128];

#define ASCII_SIZE ((i32)('~' - ' '))

#define getbit(N, X, Y) (((u64)(N)) & ((u64)1 << (((u64)(Y)) * 8 + ((u64)(X)))))
#define setbit(N, X, Y) (((u64)(N)) | ((u64)1 << (((u64)(Y)) * 8 + ((u64)(X)))))

const char* ascii_temp[] = {
    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    "        ",

    "        "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "        "
    "   1    "
    "        ",

    "        "
    "        "
    "  1 1   "
    "  1 1   "
    "        "
    "        "
    "        "
    "        ",

    "        "
    "  1  1  "
    " 111111 "
    "  1  1  "
    "  1  1  "
    " 111111 "
    "  1  1  "
    "        ",

    "   1    "
    "  1111  "
    " 1    1 "
    "  111   "
    "     1  "
    " 1    1 "
    "  1111  "
    "   1    ",

    "  1    1"
    " 1 1  1 "
    "  1  1  "
    "    1   "
    "   1    "
    "  1  1  "
    " 1  1 1 "
    "1    1  ",

    "   1    "
    "  1 1   "
    "  1 1   "
    "   1    "
    "  1 1 1 "
    " 1   1  "
    " 1  1 1 "
    "  11    ",

    "        "
    "   1    "
    "   1    "
    "        "
    "        "
    "        "
    "        "
    "        ",

    "        "
    "    1   "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "    1   "
    "        ",

    "        "
    "   1    "
    "    1   "
    "    1   "
    "    1   "
    "    1   "
    "   1    "
    "        ",

    "        "
    "  1  1  "
    "   11   "
    "  1  1  "
    "        "
    "        "
    "        "
    "        ",

    "        "
    "        "
    "   1    "
    "   1    "
    " 11111  "
    "   1    "
    "   1    "
    "        ",

    "        "
    "        "
    "        "
    "        "
    "        "
    "   1    "
    "  1     "
    "        ",

    "        "
    "        "
    "        "
    "        "
    " 111111 "
    "        "
    "        "
    "        ",

    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    "  1     "
    "        ",

    "        "
    "      1 "
    "     1  "
    "    1   "
    "   1    "
    "  1     "
    " 1      "
    "        ",

    "        "
    "   11   "
    "  1  1  "
    " 1    1 "
    " 1    1 "
    "  1  1  "
    "   11   "
    "        ",

    "        "
    "   1    "
    "  11    "
    "   1    "
    "   1    "
    "   1    "
    "  111   "
    "        ",

    "        "
    "  1111  "
    " 1    1 "
    "     1  "
    "   11   "
    "  1     "
    " 111111 "
    "        ",

    "        "
    "  1111  "
    " 1    1 "
    "     1  "
    "   11   "
    " 1    1 "
    "  1111  "
    "        ",

    "        "
    "     1  "
    "    11  "
    "   1 1  "
    "  1  1  "
    " 111111 "
    "     1  "
    "        ",

    "        "
    " 111111 "
    " 1      "
    " 11111  "
    "      1 "
    " 1    1 "
    "  1111  "
    "        ",

    "        "
    "  1111  "
    " 1      "
    " 11111  "
    " 1    1 "
    " 1    1 "
    "  1111  "
    "        ",

    "        "
    " 111111 "
    "     1  "
    "    1   "
    "   1    "
    "  1     "
    " 1      "
    "        ",

    "        "
    "  1111  "
    " 1    1 "
    "  1111  "
    " 1    1 "
    " 1    1 "
    "  1111  "
    "        ",

    "        "
    "  1111  "
    " 1    1 "
    " 1    1 "
    "  11111 "
    "      1 "
    "  1111  "
    "        ",

    "        "
    "        "
    "        "
    "   1    "
    "        "
    "        "
    "   1    "
    "        ",

    "        "
    "        "
    "   1    "
    "        "
    "        "
    "   1    "
    "  1     "
    "        ",

    "        "
    "        "
    "     11 "
    "   11   "
    " 11     "
    "   11   "
    "     11 "
    "        ",

    "        "
    "        "
    "        "
    " 111111 "
    "        "
    " 111111 "
    "        "
    "        ",

    "        "
    "        "
    " 11     "
    "   11   "
    "     11 "
    "   11   "
    " 11     "
    "        ",

    "        "
    "  1111  "
    " 1    1 "
    "    11  "
    "  11    "
    "        "
    "  1     "
    "        ",

    "        "
    "  1111  "
    " 1    1 "
    " 1 1111 "
    " 1 1  1 "
    " 1  111 "
    " 1      "
    "  1111  ",

    "        "
    "   11   "
    "   11   "
    "  1  1  "
    "  1111  "
    " 1    1 "
    " 1    1 "
    "        ",

    "        "
    " 11111  "
    " 1    1 "
    " 11111  "
    " 1    1 "
    " 1    1 "
    " 11111  "
    "        ",

    "        "
    "  1111  "
    " 1    1 "
    " 1      "
    " 1      "
    " 1    1 "
    "  1111  "
    "        ",

    "        "
    " 11111  "
    " 1    1 "
    " 1    1 "
    " 1    1 "
    " 1    1 "
    " 11111  "
    "        ",

    "        "
    " 111111 "
    " 1      "
    " 11111  "
    " 1      "
    " 1      "
    " 111111 "
    "        ",

    "        "
    " 111111 "
    " 1      "
    " 11111  "
    " 1      "
    " 1      "
    " 1      "
    "        ",

    "        "
    "  1111  "
    " 1    1 "
    " 1      "
    " 1 1111 "
    " 1    1 "
    "  1111  "
    "        ",

    "        "
    " 1    1 "
    " 1    1 "
    " 111111 "
    " 1    1 "
    " 1    1 "
    " 1    1 "
    "        ",

    "        "
    " 11111  "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    " 11111  "
    "        ",

    "        "
    "  11111 "
    "      1 "
    "      1 "
    "      1 "
    " 1    1 "
    "  1111  "
    "        ",

    "        "
    " 1   1  "
    " 1  1   "
    " 1 1    "
    " 111    "
    " 1  1   "
    " 1   1  "
    "        ",

    "        "
    " 1      "
    " 1      "
    " 1      "
    " 1      "
    " 1      "
    " 111111 "
    "        ",

    "        "
    "1     1 "
    "11   11 "
    "1 1 1 1 "
    "1  1  1 "
    "1     1 "
    "1     1 "
    "        ",

    "        "
    " 1    1 "
    " 11   1 "
    " 1 1  1 "
    " 1  1 1 "
    " 1   11 "
    " 1    1 "
    "        ",

    "        "
    "  1111  "
    " 1    1 "
    " 1    1 "
    " 1    1 "
    " 1    1 "
    "  1111  "
    "        ",

    "        "
    " 11111  "
    " 1    1 "
    " 11111  "
    " 1      "
    " 1      "
    " 1      "
    "        ",

    "        "
    "  1111  "
    " 1    1 "
    " 1    1 "
    " 1    1 "
    " 1  1 1 "
    "  1111  "
    "      1 ",


    "        "
    " 11111  "
    " 1    1 "
    " 1 111  "
    " 1 1    "
    " 1  1   "
    " 1    1 "
    "        ",

    "        "
    "  1111  "
    " 1    1 "
    "  1     "
    "   111  "
    " 1    1 "
    "  1111  "
    "        ",
    
    "        "
    "1111111 "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "        ",

    "        "
    " 1    1 "
    " 1    1 "
    " 1    1 "
    " 1    1 "
    " 1    1 "
    "  1111  "
    "        ",

    "        "
    " 1    1 "
    " 1    1 "
    " 1    1 "
    "  1  1  "
    "  1  1  "
    "   11   "
    "        ",

    "        "
    "1     1 "
    "1     1 "
    "1  1  1 "
    "1 1 1 1 "
    "11   11 "
    "1     1 "
    "        ",

    "        "
    " 1    1 "
    "  1  1  "
    "   11   "
    "   11   "
    "  1  1  "
    " 1    1 "
    "        ",

    "        "
    "1     1 "
    " 1   1  "
    "  1 1   "
    "   1    "
    "   1    "
    "   1    "
    "        ",

    "        "
    " 111111 "
    "  1     "
    "   1    "
    "    1   "
    "     1  "
    " 111111 "
    "        ",

    "        "
    "   111  "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "   111  "
    "        ",

    "        "
    " 1      "
    "  1     "
    "   1    "
    "    1   "
    "     1  "
    "      1 "
    "        ",

    "        "
    "  111   "
    "    1   "
    "    1   "
    "    1   "
    "    1   "
    "  111   "
    "        ",

    "        "
    "   1    "
    "  1 1   "
    "        "
    "        "
    "        "
    "        "
    "        ",

    "        "
    "        "
    "        "
    "        "
    "        "
    "        "
    " 111111 "
    "        ",

    "        "
    "   1    "
    "    1   "
    "        "
    "        "
    "        "
    "        "
    "        ",

    "        "
    "        "
    "  1111  "
    "      1 "
    "  11111 "
    " 1   11 "
    "  111 1 "
    "        ",
    
    "        "
    " 1      "
    " 1      "
    " 11111  "
    " 1    1 "
    " 11   1 "
    " 1 111  "
    "        ",

    "        "
    "        "
    "        "
    "  1111  "
    " 1      "
    " 1      "
    "  1111  "
    "        ",

    "        "
    "      1 "
    "      1 "
    "  11111 "
    " 1    1 "
    " 1   11 "
    "  111 1 "
    "        ",

    "        "
    "        "
    "  1111  "
    " 1    1 "
    " 111111 "
    " 1      "
    "  11111 "
    "        ",

    "        "
    "   11   "
    "   1    "
    "  111   "
    "   1    "
    "   1    "
    "   1    "
    "        ",

    "        "
    "      1 "
    "   11 1 "
    "  1  11 "
    "  1   1 "
    "   11 1 "
    "      1 "
    "   111  ",

    "        "
    "  1     "
    "  1     "
    "  1     "
    "  111   "
    "  1  1  "
    "  1  1  "
    "        ",
    
    "        "
    "   1    "
    "        "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "        ",

    "        "
    "    1   "
    "        "
    "   11   "
    "    1   "
    "    1   "
    "    1   "
    " 111    ",

    "        "
    "  1     "
    "  1     "
    "  1  1  "
    "  1 1   "
    "  11    "
    "  1 11  "
    "        ",

    "        "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "   11   "
    "        ",

    "        "
    "        "
    "        "
    " 11  11 "
    " 1 11 1 "
    " 1    1 "
    " 1    1 "
    "        ",

    "        "
    "        "
    "        "
    " 1 111  "
    " 11  1  "
    " 1   1  "
    " 1   1  "
    "        ",

    "        "
    "        "
    "        "
    "  1111  "
    " 1    1 "
    " 1    1 "
    "  1111  "
    "        ",

    "        "
    "        "
    " 1 111  "
    " 11   1 "
    " 1    1 "
    " 11111  "
    " 1      "
    " 1      ",

    "        "
    "        "
    "  111 1 "
    " 1   11 "
    " 1    1 "
    "  11111 "
    "      1 "
    "      1 ",

    "        "
    "        "
    "        "
    " 1 111  "
    " 11   1 "
    " 1      "
    " 1      "
    "        ",

    "        "
    "        "
    "   111  "
    "  1     "
    "   11   "
    "     1  "
    "  111   "
    "        ",

    "        "
    "        "
    "   1    "
    "  1111  "
    "   1    "
    "   1    "
    "   11   "
    "        ",

    "        "
    "        "
    "        "
    " 1    1 "
    " 1    1 "
    " 1   11 "
    "  111 1 "
    "        ",

    "        "
    "        "
    "        "
    " 1    1 "
    " 1    1 "
    "  1  1  "
    "   11   "
    "        ",

    "        "
    "        "
    "        "
    "1      1"
    "1      1"
    " 1 11 1 "
    " 11  11 "
    "        ",

    "        "
    "        "
    "        "
    " 1    1 "
    "  1  1  "
    "   11   "
    " 11  11 "
    "        ",

    "        "
    "        "
    " 1    1 "
    "  1   1 "
    "   1 1  "
    "    1   "
    "   1    "
    " 11     ",

    "        "
    "        "
    "        "
    " 111111 "
    "    1   "
    "   1    "
    " 111111 "
    "        ",

    "        "
    "    11  "
    "    1   "
    "   11   "
    "   11   "
    "    1   "
    "    11  "
    "        ",

    "        "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "   1    "
    "   1    ",

    "        "
    "  11    "
    "   1    "
    "   11   "
    "   11   "
    "   1    "
    "  11    "
    "        ",
    
    "        "
    "        "
    "        "
    "  11  1 "
    " 1  11  "
    "        "
    "        "
    "        "
};


static void initBitmaps() {
    memset(bitascii, 0, sizeof (u64) * ('~' - ' '));

    for (i32 i = 0; i < ('~' - ' '); i++) {
        u64 nr = 0;
        for (u64 j = 0; j < 64; j++) {
            if (ascii_temp[i][j] == '1')
                nr = nr | (((u64)(1) << j));
        }
        bitascii[i] = nr;
    }
}

static __INLINE void renderAscii(char c, r32 px, r32 py, r32 x_scale, r32 y_scale) {
    if (c >= ' ' && c <= '~') {
        u64 n = bitascii[c - ' '];
        FOR_MATRIX(8, 8) {
            if (getbit(n, x, y) > 0) {
                framebufferDrawRect(
                        (i32)(px + x * x_scale),
                        (i32)(py + y * y_scale),
                        (i32)(px + (x + 1) * x_scale),
                        (i32)(py + (y + 1) * y_scale));
            }
        }
    }
}

static __INLINE void renderAsciiString(const char* str, r32 x, r32 y, r32 scale_x, r32 scale_y)
{
    for (i32 i = 0; str[i] != '\0'; i++) {
        renderAscii(str[i], x + i * 8 * scale_x, y, scale_x, scale_y);
    }
}

static inline void renderStringBox(const char* str, r32 x, r32 y, r32 w, r32 h) {
    i32 slen = strlen(str);
    r32 x_scale = w / ((r32)slen * 8.0f);
    r32 y_scale = h / 8.0f;

    for (i32 i = 0; i < slen; i++) {
        renderAscii(str[i], x + i * 8 * x_scale, y, x_scale, y_scale);
    }
}

static inline void renderNumberBox(i32 n, r32 x, r32 y, r32 w, r32 h) {
    static char buffer[32];

    sprintf(buffer, "%d", n);

    renderStringBox(buffer, x, y, w, h);
}

#endif

