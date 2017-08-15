#include "system_sam3x.h"
#include "at91sam3x8.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

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
    r32 min[7];
    r32 max[7];
    r32 avg[7];

    ARRAY(u16, MAX_TEMPS) array;
} temp;

static u16 tempNoise(float x) {
    r32 noise = perlinNoise(x * 0.01f, 0.0f, 0.0f);
    return 30000 + (u16)(12000.0f * noise);
}

static void randomTempArray() {
    CLEAR(&temp.array);

    for (i32 i = 0; i < DAYS; i++) {
        for (i32 j = 0; j < MINUTES; j++) {
            u16 noise = tempNoise(i);

            ADD(&temp.array, noise);
        }
    }
}

// ===================================== MAIN MENU ======================================== //

STRUCT(MenuOption) { const char *name; void (*runSystem)(); };

static const MenuOption menu[] = {
    { "random temp", randomTempArray },
    { "play snake",  runSnakeGame }
};

static const i32 menuSize = sizeof (menu) / sizeof(MenuOption);

static void mainMenu() {
    char strBuffer[16];

    r32 spacing = 16.0f;

    i32 cursor = 0;

    while (1) {
        framebufferClear();
        renderStringBox("Main Menu", 0.0f, 0.0f, DISPLAY_PIXEL_WIDTH, DISPLAY_PIXEL_HEIGHT / 10);

        sprintf(strBuffer, "temp: %.2f", tempGetCurrent());
        renderAsciiString(strBuffer, 132, 16, 1.0f, 1.0f);

        sprintf(strBuffer, "data: %d", SIZE(&temp.array));
        renderAsciiString(strBuffer, 132, 32, 1.0f, 1.0f);

        sprintf(strBuffer, "max:  %d",   MAX_TEMPS);
        renderAsciiString(strBuffer, 132, 48, 1.0f, 1.0f);

        for (i32 i = 0; i < menuSize; i++) {
            renderAsciiString(menu[i].name, 16, 16 + i * spacing, 1.0f, 1.0f);
        }

        renderAscii('*', 0, 16 + cursor * spacing, 1.0f, 1.0f);

        switch (keypadRead()) {
            case 5: { menu[cursor].runSystem(); } break;
            case 2: { cursor--; } break;
            case 8: { cursor++; } break;
            case 1: { spacing -= 1; } break;
            case 3: { spacing += 1; } break;
        }

        if (cursor < 0) { cursor = menuSize - 1; }
        if (cursor >= menuSize) { cursor = 0; }

        framebufferClearPixel(0, 1);
        framebufferClearPixel(0, 2);
        framebufferClearPixel(0, 3);

        framebufferDisplay();

        delay(10000);
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

    initBitmaps();

    tempStartMesurment();
    mainMenu();
}

