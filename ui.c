/** @file ui.c
    @author Alex Belcher, Joel Bremner
    @date 10 October 2023
    @brief displays end message module
*/

#include "ui.h"
#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "screen.h"

#define MESSAGE_RATE 10
#define PACER_RATE 500

void end_game(char character) 
{
    /**
    Sends through the character to be displayed
    */

    // Display letter
    uint32_t timer_count = 0;
    while (1) {
        // After set time return to main to play again
        timer_count += 1;
        if (timer_count == 1000) {
            return;
        }
        
        pacer_wait ();
        tinygl_update ();
        display_character (character);
    }
}

void start_game_ui(void)
{
    /**
    Display start of game screen
    */
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font5x7_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text("PONG");
}
