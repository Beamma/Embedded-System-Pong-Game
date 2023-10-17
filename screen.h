/** @file screen.h
    @author Alex Belcher, Joel Bremner
    @date 6 October 2023
    @brief led screen set up support
*/

#ifndef SCREEN_H
#define SCREEN_H

#include "system.h"
#include "tinygl.h"
#include "pio.h"

void display_column(uint8_t row_pattern, uint8_t current_column);

void led_init(void);

void display_character (char character);





#endif