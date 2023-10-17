/** @file screen.c
    @author Alex Belcher, Joel Bremner
    @date 6 October 2023
    @brief led screen set up module
*/

#include "screen.h"
#include "system.h"
#include "tinygl.h"
#include "pio.h"

/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO, 
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};

/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

static uint8_t last_col = 0;

void display_column(uint8_t row_pattern, uint8_t current_column)
{
    /**
    Displays the current column of the ball
    */
    pio_output_high(cols[last_col]); 

    for(int i = 0; i < 7;  i++) {
        if((row_pattern >> i) & 1) {
            pio_output_low(rows[i]);
        } else {
            pio_output_high(rows[i]);
        }
    }
    pio_output_low(cols[current_column]); 

    last_col = current_column;
}

void led_init(void)
{
    /**
    initialise LEDS, all LEDs are off
    */ 
    for (int i = 0; i < 7; i++) { // Loop through rows
        pio_config_set(rows[i], PIO_OUTPUT_HIGH);
    }

    for (int i = 0; i < 5; i++) { // Loop through columns
        pio_config_set(cols[i], PIO_OUTPUT_HIGH);
    }
}

void display_character (char character)
{
    /**
    Displays the result of the game (W)in or (L)ose
    */
    char buffer[2];

    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}