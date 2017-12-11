#include "system_sam3x.h"
#include "at91sam3x8.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <inttypes.h>

#include "ats.h"

#include "systick.h"
#include "delay.h"
#include "keypad.h"
#include "display.h"
#include "temp.h"
#include "servo.h"
#include "light_sensor.h"

#include "bitmaps.h"
#include "framebuffer.h"
#include "snake.h"

#define DAYS        (7)
#define MINUTES     (60 * 24)
#define MAX_TEMPS   (DAYS * MINUTES)

#define TEMP_INDEX(M, D)    ((D) * MINUTES + (M))

static Array(u16, MAX_TEMPS) temp_array;

static void loggCurrentTemp() {
    static int last = 0;

    i32 t = systickGetTime();

    if (t > (last + 1000)) {
        array_add(&temp_array, tempGetCompressed());
        last = t;
    }
}

static u16 tempNoise(r32 x) {
    r32 noise = perlinNoise(x * 0.5f + 10000.0f, 0.5f, 0.0f);
    return 30000 + (u16)(15000.0f * noise);
}

static void randomTempArray() {
    array_clear(&temp_array);
    
    for (i32 i = 0; i < DAYS; i++) {
        for (i32 j = 0; j < MINUTES; j++) {
            u16 n = tempNoise(i);
            array_add(&temp_array, n);
        }
    }
}

static void startLogging()  { isLogging = 1; }
static void stopLogging()   { isLogging = 0; }
static void clearLog()      { array_clear(&temp_array); }

static void showTempSum() {
    Array(r32, 7) min;
    Array(r32, 7) max;
    Array(r32, 7) avg;

    array_clear(&min);
    array_clear(&max);
    array_clear(&avg);

    for (i32 i = 0; (i < DAYS) && (TEMP_INDEX(0, i) < array_size(&temp_array)); i++) {
        r32 temp = tempDecompress(array_get(&temp_array, TEMP_INDEX(0, i))); 

        r32 tmin = temp;
        r32 tmax = temp;
        r32 tavg = temp;

        i32 j;
        for (j = 1; (j < MINUTES) && (TEMP_INDEX(j, i) < array_size(&temp_array)); j++) {
            temp = tempDecompress(array_get(&temp_array, TEMP_INDEX(j, i))); 

            tmin = fminf(temp, tmin);
            tmax = fmaxf(temp, tmax);

            tavg += temp;
        }

        array_add(&min, tmin);
        array_add(&max, tmax);
        array_add(&avg, tavg / (r32)MINUTES);

        printf("%f %f %f\n", tmin, tmax, tavg / (r32)MINUTES);
    }

    b32 running = 1;
    while (running) {
        if (keypadRead() == 1) { running = 0; }
        // draw interface
        renderString("Summary:", 0, 0, 2.0f, 2.0f);

        renderString("mon", 30, 0 * 12 + 32, 1, 1);
        renderString("tue", 30, 1 * 12 + 32, 1, 1);
        renderString("whe", 30, 2 * 12 + 32, 1, 1);
        renderString("thu", 30, 3 * 12 + 32, 1, 1);
        renderString("fri", 30, 4 * 12 + 32, 1, 1);
        renderString("sat", 30, 5 * 12 + 32, 1, 1);
        renderString("sun", 30, 6 * 12 + 32, 1, 1);

        renderString("min", 64,  18, 1, 1);
        renderString("max", 128, 18, 1, 1);
        renderString("avg", 192, 18, 1, 1);

        // draw data
        for (i32 i = 0; i < array_size(&min); i++) {
            renderR32(array_get(&min, i), 64,  32 + 12 * i, 1, 1);
            renderR32(array_get(&max, i), 128, 32 + 12 * i, 1, 1);
            renderR32(array_get(&avg, i), 192, 32 + 12 * i, 1, 1);
        }

        framebufferDisplay();
        delay(1000000);
    }
}

static void showTempGraph() {
    r32 temp = tempDecompress(array_get(&temp_array, 0));

    r32 min = temp;
    r32 max = temp;

    for (i32 i = 1; i < array_size(&temp_array); i++) {
        temp = tempDecompress(array_get(&temp_array, i));

        min  = fminf(temp, min);
        max  = fmaxf(temp, max);
    }

    i32 x = 0;
    r32 h = max - min;

    while (keypadRead() != 1) {
        if (keypadRead() == 6) { x++; }
        for (i32 i = 0; i < DISPLAY_PIXEL_WIDTH; i++) {
            r32 temp = tempDecompress(array_get(&temp_array, x + i));

            temp = (min < 0.0f? temp + min : temp - min) / h;

            framebufferSetPixel(i, DISPLAY_PIXEL_HEIGHT - (i32)(temp));
        }

        framebufferDisplay();
    }
}

// ===================================== MAIN MENU ======================================== //

STRUCT(MenuOption) { const char *name; void (*run)(); };

static const MenuOption menu[] = {
    { "start logging", startLogging },
    { "stop logging",  stopLogging },
    { "clear logg",    clearLog },
    { "show summary",  showTempSum },
    { "show graph",    showTempGraph },
    { "random temp",   randomTempArray },
    { "play snake",    runSnakeGame }
};

static const i32 menuSize = sizeof (menu) / sizeof(MenuOption);

static void mainMenu() {
    char strBuffer[16];
    
    r32 spacing = 16.0f;
    i32 cursor = 0;
    
    while (1) {
        renderStringBox("Main Menu", 0.0f, 0.0f, DISPLAY_PIXEL_WIDTH, DISPLAY_PIXEL_HEIGHT / 10);
        
        sprintf(strBuffer, "temp: %.2f", tempGetCurrent());
        renderString(strBuffer, 132, 16, 1.0f, 1.0f);
        
        sprintf(strBuffer, "data: %d", array_size(&temp_array));
        renderString(strBuffer, 132, 32, 1.0f, 1.0f);
        
        sprintf(strBuffer, "max:  %d",   MAX_TEMPS);
        renderString(strBuffer, 132, 48, 1.0f, 1.0f);
        
        for (i32 i = 0; i < menuSize; i++) {
            renderString(menu[i].name, 16, 16 + i * spacing, 1.0f, 1.0f);
        }
        
        renderString("->", 0, 16 + cursor * spacing, 1.0f, 1.0f);
        
        switch (keypadRead()) {
            case 5: { if (menu[cursor].run) menu[cursor].run(); } break;
            case 2: { cursor--; } break;
            case 8: { cursor++; } break;
            case 1: { spacing -= 2; } break;
            case 3: { spacing += 2; } break;
        }
        
        if (cursor < 0) { cursor = menuSize - 1; }
        if (cursor >= menuSize) { cursor = 0; }
        
        framebufferDisplay();
        
        delay(1000000);
    }
}

// ====================================== MAIN FUNCTION ==================================== //

int main() {
    SystemInit();

    initSystick();
    initKeypad();
    initDisplay();
    initServo();
    initTemp();
    initLightSensor();

    tempStartMesurment();
   
    mainMenu();
}

