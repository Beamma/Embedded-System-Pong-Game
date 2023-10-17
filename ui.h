/** @file ui.h
    @author Alex Belcher, Joel Bremner
    @date 10 October 2023
    @brief displays end message support
*/


#ifndef UI_H
#define UI_H

#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"
#include "screen.h"

void end_game(char character);

void start_game_ui(void);

#endif