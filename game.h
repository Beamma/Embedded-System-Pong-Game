/** @file game.h
    @author Alex Belcher, Joel Bremner
    @date 6 October 2023
    @brief Initialises the game support
*/

#ifndef GAME_H
#define GAME_H

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "ir_uart.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "pio.h"
#include <stdbool.h>
#include <stdio.h>
#include "ball.h"
#include "screen.h"

#define MESSAGE_RATE 10

#define PACER_RATE 500

typedef struct {
    int8_t x;
    int8_t y;
    uint32_t v;
    int8_t angle;
    bool direction;

} Ball_t;

void play(void);

void start_game(Ball_t* ball);

int main (void);

#endif