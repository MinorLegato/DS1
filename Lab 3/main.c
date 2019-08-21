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

static void log_current_temp() {
    static int last = 0;

    i32 t = systick_get_time();

    if (t > (last + 1000)) {
        array_add(&temp_array, temp_get_compressed());
        last = t;
    }
}

static u16 temp_noise(f32 x) {
    f32 noise = perlinNoise(x * 0.5f + 10000.0f, 0.5f, 0.0f);
    return 30000 + (u16)(15000.0f * noise);
}

static void temp_random() {
    array_clear(&temp_array);
    
    for (int i = 0; i < DAYS; i++) {
        for (int j = 0; j < MINUTES; j++) {
            u16 n = temp_noise(i);
            array_add(&temp_array, n);
        }
    }
}

static void log_start()  { is_logging = 1; }
static void log_stop()   { is_logging = 0; }
static void log_clear()  { array_clear(&temp_array); }

static void temp_show_sum() {
    Array(f32, 7) min;
    Array(f32, 7) max;
    Array(f32, 7) avg;

    array_clear(&min);
    array_clear(&max);
    array_clear(&avg);

    for (i32 i = 0; (i < DAYS) && (TEMP_INDEX(0, i) < array_size(&temp_array)); i++) {
        f32 temp = temp_decompress(array_get(&temp_array, TEMP_INDEX(0, i))); 

        f32 tmin = temp;
        f32 tmax = temp;
        f32 tavg = temp;

        for (int j = 1; (j < MINUTES) && (TEMP_INDEX(j, i) < array_size(&temp_array)); j++) {
            temp = temp_decompress(array_get(&temp_array, TEMP_INDEX(j, i))); 

            tmin = fminf(temp, tmin);
            tmax = fmaxf(temp, tmax);

            tavg += temp;
        }

        array_add(&min, tmin);
        array_add(&max, tmax);
        array_add(&avg, tavg / (f32)MINUTES);

        printf("%f %f %f\n", tmin, tmax, tavg / (f32)MINUTES);
    }

    int running = 1;
    while (running) {
        if (keypad_read() == 1) { running = 0; }
        // draw interface
        render_string("Summary:", 0, 0, 2.0f, 2.0f);

        render_string("mon", 30, 0 * 12 + 32, 1, 1);
        render_string("tue", 30, 1 * 12 + 32, 1, 1);
        render_string("whe", 30, 2 * 12 + 32, 1, 1);
        render_string("thu", 30, 3 * 12 + 32, 1, 1);
        render_string("fri", 30, 4 * 12 + 32, 1, 1);
        render_string("sat", 30, 5 * 12 + 32, 1, 1);
        render_string("sun", 30, 6 * 12 + 32, 1, 1);

        render_string("min", 64,  18, 1, 1);
        render_string("max", 128, 18, 1, 1);
        render_string("avg", 192, 18, 1, 1);

        // draw data
        for (int i = 0; i < array_size(&min); i++) {
            render_float(array_get(&min, i), 64,  32 + 12 * i, 1, 1);
            render_float(array_get(&max, i), 128, 32 + 12 * i, 1, 1);
            render_float(array_get(&avg, i), 192, 32 + 12 * i, 1, 1);
        }

        framebuffer_display();
        delay(1000000);
    }
}

static void temp_show_graph() {
    f32 temp = temp_decompress(array_get(&temp_array, 0));

    f32 min = temp;
    f32 max = temp;

    for (int i = 1; i < array_size(&temp_array); i++) {
        temp = temp_decompress(array_get(&temp_array, i));

        min  = fminf(temp, min);
        max  = fmaxf(temp, max);
    }

    int x = 0;
    f32 h = max - min;

    while (keypad_read() != 1) {
        if (keypad_read() == 6) { x++; }
        for (int i = 0; i < DISPLAY_PIXEL_WIDTH; i++) {
            f32 temp = temp_decompress(array_get(&temp_array, x + i));

            temp = (min < 0.0f? temp + min : temp - min) / h;

            framebuffer_set_pixel(i, DISPLAY_PIXEL_HEIGHT - (int)(temp));
        }

        framebuffer_display();
    }
}

// ===================================== MAIN MENU ======================================== //

typedef struct Menu_Option {
    const char *name;
    void (*run)();
} Menu_Option;

static const Menu_Option menu[] = {
    { "start logging", log_start },
    { "stop logging",  log_stop },
    { "clear logg",    log_clear },
    { "show summary",  temp_show_sum },
    { "show graph",    temp_show_graph },
    { "random temp",   temp_random },
    { "play snake",    snake_run }
};

static const int menu_size = sizeof (menu) / sizeof(Menu_Option);

static void mainMenu() {
    char str_buffer[16];
    f32  spacing = 16.0f;
    i32  cursor = 0;
    
    while (1) {
        render_string_box("Main Menu", 0.0f, 0.0f, DISPLAY_PIXEL_WIDTH, DISPLAY_PIXEL_HEIGHT / 10);
        
        sprintf(str_buffer, "temp: %.2f", temp_get_current());
        render_string(str_buffer, 132, 16, 1.0f, 1.0f);
        
        sprintf(str_buffer, "data: %d", array_size(&temp_array));
        render_string(str_buffer, 132, 32, 1.0f, 1.0f);
        
        sprintf(str_buffer, "max:  %d",   MAX_TEMPS);
        render_string(str_buffer, 132, 48, 1.0f, 1.0f);
        
        for (int i = 0; i < menu_size; i++) {
            render_string(menu[i].name, 16, 16 + i * spacing, 1.0f, 1.0f);
        }
        
        render_string("->", 0, 16 + cursor * spacing, 1.0f, 1.0f);
        
        switch (keypad_read()) {
            case 5: { if (menu[cursor].run) menu[cursor].run(); } break;
            case 2: { cursor--; } break;
            case 8: { cursor++; } break;
            case 1: { spacing -= 2; } break;
            case 3: { spacing += 2; } break;
        }
        
        if (cursor < 0) { cursor = menu_size - 1; }
        if (cursor >= menu_size) { cursor = 0; }
        
        framebuffer_display();
        
        delay(1000000);
    }
}

// ====================================== MAIN FUNCTION ==================================== //

int main() {
    SystemInit();

    systick_init();
    keypad_init();
    display_init();
    servo_init();
    temp_init();
    light_sensor_init();

    temp_start_mesument();
   
    mainMenu();
}

