/** @file ball.h
    @author Alex Belcher, Joel Bremner
    @date 10 October 2023
    @brief ball movement support
*/


#ifndef BALL_H
#define BALL_H

#include "system.h"
#include "ui.h"
#include "game.h"

int update_ball(Ball_t* ball,  int8_t bitmap[]);

void ball_hit_paddle(Ball_t* ball, int8_t bitmap[]);

void move_ball_away(Ball_t* ball, int8_t bitmap[]);

void move_ball_towards(Ball_t* ball, int8_t bitmap[]);

void send_ball(Ball_t* ball);

int recieve_ball(Ball_t* ball);

#endif