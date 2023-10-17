/** @file paddle.c
    @author Alex Belcher, Joel Bremner
    @date 10 October 2023
    @brief Paddle movement module
*/

#include "paddle.h"
#include "navswitch.h"
#include "system.h"

void update_paddle(uint8_t* current_column, int8_t bitmap[])
{
    /**
    Updates the display to the correct position of the paddle
    */

    // Moves paddle North
    if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            if (bitmap[4] > 0x07) {
                bitmap[4] = ((bitmap[4] >> 1) & 0xFF);
                *current_column = 0;
            }
        }

    // Moves paddle South
    if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {   
        if (bitmap[4] < 0x70) {
            bitmap[4] = ((bitmap[4] << 1) & 0xFF);
            *current_column = 0;
        }
    }
}