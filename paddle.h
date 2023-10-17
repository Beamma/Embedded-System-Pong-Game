/** @file paddle.h
    @author Alex Belcher, Joel Bremner
    @date 10 October 2023
    @brief Paddle movement support
*/

#ifndef PADDLE_H
#define PADDLE_H

#include "navswitch.h"
#include "system.h"


void update_paddle(uint8_t* current_column, int8_t bitmap[]);



#endif