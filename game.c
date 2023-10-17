/** @file game.c
    @author Alex Belcher, Joel Bremner
    @date 6 October 2023
    @brief Initialises the game module
*/

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
#include "paddle.h"
#include "ui.h"

#define MESSAGE_RATE 10
#define PACER_RATE 500

void play(void)
{   
    /**
    Initialises ball, starts game, and updates game status
    */
    // Initilisation Of Variables
    uint8_t current_column = 0;
    int8_t bitmap[] =
    {
        0x0, 0x0, 0x0, 0x0, 0x1C
    };
    Ball_t ball = 
    {
        0x00, 0, 0, 0, 1
    };

    start_game(&ball);

    // Main Game Loop
    uint32_t timer_count = 0;
    while (1) {
        // Reicieve byte via IR from other player
        if (ir_uart_read_ready_p ()) {    
            int status = recieve_ball(&ball);
            // If game is over return to main to play again
            if (status == 1) {
                return;
            }
            ball.direction = 1;
            timer_count = 0;
        }

        timer_count += 1;

        // Update ball
        if (timer_count == (ball.v * 150)) {
            timer_count = 0;
            int status = update_ball(&ball, bitmap);
            // If game is over return to main to play again
            if (status == 1) {
                return;
            }
        }

        pacer_wait ();
        navswitch_update ();
        display_column(bitmap[current_column], current_column);
        update_paddle(&current_column, bitmap);
    
        // Change through columns to display
        current_column++;
        if (current_column > (LEDMAT_COLS_NUM-1)) {
            current_column = 0;
        }
    }
}

void start_game(Ball_t* ball)
{   
    /**
    Waits for both players to press nav down button, then initialises game
    */
    bool sent = false;
    bool recv = false;
    start_game_ui();

    while(1) {   
        navswitch_update ();
        pacer_wait ();
        tinygl_update();
        // Other player ready
        if (ir_uart_read_ready_p ()) {
            int8_t byte;
            byte = ir_uart_getc ();
            if (byte == 1) {
                if (sent == true) {
                    ball->x = 0x08;
                    ball->v = 0x01;
                    break;
                } else {
                    recv = true;
                }
            }
        }

        // Your ready
        if (navswitch_push_event_p(NAVSWITCH_PUSH)) {
            ir_uart_putc(1);
            sent = true;
        }
        
        // If both ready
        if (sent == true && recv == true) {
            break;
        }
    }
}

int main (void)
{
    /**
    Initialises, and begins the first steps of starting the game
    */
   while (1) {
    system_init ();
    pacer_init (PACER_RATE);
    led_init();
    ir_uart_init ();
    // Play Game
    play();
   }
    
}

