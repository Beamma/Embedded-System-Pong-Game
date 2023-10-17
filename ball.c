/** @file ball.c
    @author Alex Belcher, Joel Bremner
    @date 10 October 2023
    @brief ball movement module
*/


#include "ball.h"
#include "system.h"
#include "ui.h"
#include "game.h"
#include "ir_uart.h"
#include <math.h>

#define PADDLE_ROW 4
#define LEFT 2
#define RIGHT 1 
#define STRAIGHT 0



int update_ball(Ball_t* ball, int8_t bitmap[])
{
    /**
    Takes pointer to ball type and an array, and calculates the next position of the ball
    */

    // End game if ball hits baseline
    if (ball->y == 5) { 
        ir_uart_putc(0x80); // Send end game byte
        char character = 'L';
        end_game(character);
        return 1;
    }

    // Ball Heading towards Paddle
    if (ball->direction == 1) {
        // Remove previous pixel from screen
        if (ball->y >= 1) {
            bitmap[ball->y-1] = 0x0;
        }
        // Display ball in same row as paddle
        if (ball->y == PADDLE_ROW) {
            bitmap[PADDLE_ROW] = (bitmap[PADDLE_ROW] | ball->x);
            ball->y += 1;
        } else {
            move_ball_towards(ball, bitmap);
            // Check if ball hits paddle
            if (ball->y == PADDLE_ROW) {
                if ((ball->x & bitmap[PADDLE_ROW]) != 0) {
                    ball_hit_paddle(ball, bitmap);
                }
            }
        }
    
    // Ball Heading away from paddle
    } else {
        // Remove previous pixel from screen
        if (ball->y <= 2) {
            bitmap[ball->y+1] = 0x0;
        }

        // If ball reaches net send ball via IR, then remove ball from players screen
        if (ball->y == -1) {
            ball->y = 0;
            send_ball(ball);
            ball->x = 0x00;
            ball->v = 0x00;
            bitmap[0] = 0x00;
        }
        
        // Angles And Movement Of Ball
        move_ball_away(ball, bitmap);
    }
    return 0;
}

void send_ball(Ball_t* ball)
{   
    /** 
    Send the balls info via IR to opposing player and updates the ball type
    */
    int8_t ball_info = 0x00;
    int8_t x_val = ball->x;

    // Manafacture Byte
    int8_t position = log10(x_val) / log10(2); // Calculate index of ball in bytemap, using log base 2
    ball_info = (ball_info | (position << 4)); // Bitfiddle X position of ball
    ball_info = (ball_info | (ball->angle << 2)); // Bitfiddle angle Of Ball

    ir_uart_putc(ball_info);
}

int recieve_ball(Ball_t* ball)
{   
    /** 
    Recieves ball info via IR from opposing player and updates the ball type
    */
    uint8_t ball_info;
    ball_info = ir_uart_getc();

    // Check to see if recieves end game
    if (ball_info == 0x80) {
        char character = 'W';
        end_game(character);
        return 1;
    }

    // Decompose Byte
    int8_t x_val = ((0x70 & ball_info) >> 4); // Get x index of ball in byte array
    int8_t angle = ((0x0C & ball_info) >> 2); // Get angle of ball
    
    x_val = 6 - x_val; // Mirror X value

    // Mirror angles
    if (angle == LEFT) {
        angle = RIGHT;
        x_val += 1;
    } else if (angle == RIGHT) {
        angle = LEFT;
        x_val -= 1;
    }

    ball->angle = angle;
    ball->x = (0x01 << x_val); // Set pixel in correct index of byte array
    ball->v = 0x01;

    return 0;
}

void ball_hit_paddle(Ball_t* ball, int8_t bitmap[])
{   
    /**
    Calculate if ball has hit paddle, if so alter angle and direction of ball accordingly
    */
    if (((bitmap[PADDLE_ROW] << 1) & ball->x) == 0) { // Move left
        ball->angle = RIGHT;
    } else if (((bitmap[PADDLE_ROW] >> 1) & ball->x) == 0) { // Move right
        ball->angle = LEFT;
    } else {
        ball->angle = STRAIGHT;
    }

    ball->direction = 0;
    ball->y = 2;
}

void move_ball_away(Ball_t* ball, int8_t bitmap[])
{   
    /**
    Updates the balls position and converts this to a bitmap to be displayed (Heading away from paddle)
    */
    // Check Angles
    if (ball->angle == RIGHT) { // RIGHT
        if (ball->x == 1) { // Hit wall
            ball->angle = LEFT;
            ball->x = (ball->x << 1);
        } else {
            ball->x = (ball->x >> 1);
        }
        bitmap[ball->y] = ball->x;
    } else if (ball->angle == LEFT) { // LEFT
        if (ball->x == 64) { // hit wall
            ball->angle = RIGHT;
            ball->x = (ball->x >> 1);
        } else {
            ball->x = (ball->x << 1);
        }

        bitmap[ball->y] = ball->x;
    } else {
        bitmap[ball->y] = ball->x;
    }

    ball->y -= 1;
}

void move_ball_towards(Ball_t* ball, int8_t bitmap[])
{   
    /**
    Updates the balls position and converts this to a bitmap to be displayed (Heading towards paddle)
    */
    // Check Angles
    if (ball->angle == RIGHT) { // RIGHT
        if (ball->x == 1) { // Hit wall
            ball->angle = LEFT;
            ball->x = (ball->x << 1);
        } else {
            ball->x = (ball->x >> 1);
        }

        bitmap[ball->y] = ball->x;

    } else if (ball->angle == LEFT) { // LEFT
        if (ball->x == 64) { // Hit wall
            ball->angle = RIGHT;
            ball->x = (ball->x >> 1);
        } else {
            ball->x = (ball->x << 1);
        }

        bitmap[ball->y] = ball->x;
        
    } else {
        bitmap[ball->y] = ball->x;
    }

    ball->y += 1;
}