#ifndef __SNAKE_H__
#define __SNAKE_H__

#include "ats.h"

#include <stdlib.h>
#include <stdint.h>
#include <time.h>


#include "framebuffer.h"
#include "keypad.h"


STRUCT(SnakeState) {
    i16 head;
    i16 tail;
    
    i8 dir_x;
    i8 dir_y;
    
    i8 fdir_x;
    i8 fdir_y;
    
    i8 apple_x;
    i8 apple_y;
    
    i8 xs[DISPLAY_SIZE];
    i8 ys[DISPLAY_SIZE];
    
    u32 board[DISPLAY_SIZE];
};

static void initSnakeState(SnakeState* snake) {
    memset(snake, 0, sizeof *snake);
    
    snake->dir_y  = -1;
    snake->fdir_x = -snake->dir_x;
    snake->fdir_y = -snake->dir_y;
    
    snake->xs[snake->head] = DISPLAY_WIDTH / 2 - 1;
    snake->ys[snake->head] = DISPLAY_HEIGHT / 2 + 2;
    
    do {
        snake->apple_x = rand() % DISPLAY_WIDTH;
        snake->apple_y = rand() % DISPLAY_HEIGHT;
    } while (snake->xs[0] == snake->apple_x && snake->ys[0] == snake->apple_y);
}

static __INLINE clock_t elapsedTime(clock_t* start) {
    return clock() - *start;
}

static __INLINE void resetTimer(clock_t* time) {
    *time = clock();
}

void runSnakeGame() {
    static SnakeState snake;

    i32 game_running = 1;
    clock_t timer = clock();

    initSnakeState(&snake);
    
    display_clear();
    framebufferClear();

    while (game_running) {
        i32 key = keypadRead();

        if (key) {
            if (key == 10) game_running = 0;
            if (key == 2 && snake.fdir_y != -1) { snake.dir_x =  0; snake.dir_y = -1; }
            if (key == 8 && snake.fdir_y !=  1) { snake.dir_x =  0; snake.dir_y =  1; }
            if (key == 4 && snake.fdir_x != -1) { snake.dir_x = -1; snake.dir_y =  0; }
            if (key == 6 && snake.fdir_x !=  1) { snake.dir_x =  1; snake.dir_y =  0; }
        }

        if (elapsedTime(&timer) > 25) {
            resetTimer(&timer);
            
            snake.fdir_x = -snake.dir_x;
            snake.fdir_y = -snake.dir_y;
            
            i32 old_head = snake.head;

            framebufferClearSnake(snake.xs[snake.tail], snake.ys[snake.tail]);

            snake.board[snake.ys[snake.tail]] ^= (1 << snake.xs[snake.tail]);
            
            if (++snake.head == DISPLAY_SIZE) { snake.head = 0; }
            if (++snake.tail == DISPLAY_SIZE) { snake.tail = 0; }
            
            snake.xs[snake.head] = snake.xs[old_head];
            snake.ys[snake.head] = snake.ys[old_head];
            
            snake.xs[snake.head] += snake.dir_x;
            snake.ys[snake.head] += snake.dir_y;
            
            if (snake.xs[snake.head] == snake.apple_x && snake.ys[snake.head] == snake.apple_y) {
                do {
                    snake.apple_x = rand() % DISPLAY_WIDTH;
                    snake.apple_y = rand() % DISPLAY_HEIGHT;
                } while (snake.board[snake.apple_y] & (1 << snake.board[snake.apple_y]));
                
                if (snake.tail == 0) { snake.tail = DISPLAY_SIZE - 1; }
                else                 { snake.tail--; }
                
                snake.board[snake.ys[snake.tail]] ^= (1 << snake.xs[snake.tail]);
            }
            
            if (snake.board[snake.ys[snake.head]] & (1 << snake.xs[snake.head]))
                game_running = 0;
            
            if (snake.xs[snake.head] < 0 || snake.ys[snake.head] < 0 ||
                snake.xs[snake.head] >= DISPLAY_WIDTH ||
                snake.ys[snake.head] >= DISPLAY_HEIGHT) {
                game_running = 0;
            }
            
            snake.board[snake.ys[snake.head]] ^= (1 << snake.xs[snake.head]);
            
            framebufferDrawSnake(snake.xs[snake.head], snake.ys[snake.head]);
            framebufferDrawApple(snake.apple_x, snake.apple_y);

            framebufferDisplay();
        }
    }
}


#endif
