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

static struct {
    ARRAY(r32, 7) min;
    ARRAY(r32, 7) max;
    ARRAY(r32, 7) avg;
    
    ARRAY(u16, MAX_TEMPS) array;
} temp;

static u16 tempNoise(r32 x) {
    r32 noise = perlinNoise(x * 0.1f, 0.1f, 0.0f);
    return 30000 + (u16)(12000.0f * noise);
}

static void randomTempArray() {
    CLEAR(&temp.min);
    CLEAR(&temp.max);
    CLEAR(&temp.avg);
    
    CLEAR(&temp.array);
    
    for (i32 i = 0; i < DAYS; i++) {
        r32 min = 100;
        r32 max = -100;
        r32 avg = 0;
        
        for (i32 j = 0; j < MINUTES; j++) {
            u16 n = tempNoise(i);
            ADD(&temp.array, n);
            
            r32 t = tempDecompress(n);
            if (t < min) min = t;
            if (t > max) max = t;
            avg += t;
        }
        
        ADD(&temp.min, min);
        ADD(&temp.max, max);
        ADD(&temp.avg, avg / MINUTES);
    }
}

static void startLogging() {
    // TODO(anton):
}

static void stopLogging() {
    // TODO(anton):
}

static void clearLog() {
    // TODO(anton):
    CLEAR(&temp.min);
    CLEAR(&temp.max);
    CLEAR(&temp.avg);
    CLEAR(&temp.array);
}

static void showTempSum() {
    b32 running = 1;
    while (running) {
        if (keypadRead() == 1) { running = 0; }
        renderString("Summary:", 0, 0, 2.0f, 2.0f);
        for (i32 i = 0; i < SIZE(&temp.min); i++) {
            renderI32(GET(&temp.min, i), 32 * i, 32, 1.0f, 1.0f);
            renderI32(GET(&temp.max, i), 32 * i, 64, 1.0f, 1.0f);
            renderI32(GET(&temp.avg, i), 32 * i, 98, 1.0f, 1.0f);
        }
        framebufferDisplay();
        delay(1000000);
    }
}

static void showTempGraph() {
    // TODO(anton):
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
        
        sprintf(strBuffer, "data: %d", SIZE(&temp.array));
        renderString(strBuffer, 132, 32, 1.0f, 1.0f);
        
        sprintf(strBuffer, "max:  %d",   MAX_TEMPS);
        renderString(strBuffer, 132, 48, 1.0f, 1.0f);
        
        
        for (i32 i = 0; i < menuSize; i++) {
            renderString(menu[i].name, 16, 16 + i * spacing, 1.0f, 1.0f);
        }
        
        renderAscii('*', 0, 16 + cursor * spacing, 1.0f, 1.0f);
        
        switch (keypadRead()) {
            case 5: { if (menu[cursor].run) menu[cursor].run(); } break;
            case 2: { cursor--; } break;
            case 8: { cursor++; } break;
            case 1: { spacing -= 2; } break;
            case 3: { spacing += 2; } break;
        }
        
        if (cursor < 0) { cursor = menuSize - 1; }
        if (cursor >= menuSize) { cursor = 0; }
        
        framebufferClearPixel(0, 1);
        framebufferClearPixel(0, 2);
        framebufferClearPixel(0, 3);
        framebufferClearPixel(0, 4);
        framebufferClearPixel(0, 5);
        framebufferClearPixel(0, 6);
        
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
    
    for (i32 i = 0; i < 10; i++) {
        printf("%d\n", tempNoise(100 * i));
    }
    
    mainMenu();
}

