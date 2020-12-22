#include <gb/gb.h>
#include <stdio.h>
#include <string.h>
#include "def_tiles.c"
#include "def_characters.c"
#include "def_windows.c"
#include "def_noise.c"

// --- Constants ---

#define MAX_STRING_SIZE 17

const UINT8 dia_first_row_start_adr = 0xd0;
const UINT8 dia_second_row_start_adr =  0xe0;
const UINT8 blk_tile_adr = 0xf0;
const UINT8 arr_tile_adr = 0xf1;
const UINT8 char_offset = 32;

/* An example pangram version A with 3 lines */
const unsigned char pangram_a[][MAX_STRING_SIZE] = 
{
    "Sphinx of black ", 
    "quartz, judge   ", 
    "my vow.         "
};

/* An example pangram version B with 4 lines */
const unsigned char pangram_b[][MAX_STRING_SIZE] = 
{
    "The quick brown ", 
    "fox jumps over  ", 
    "the lazy old    ",
    "dog.            "
};

// --- Indexer ---

UINT8 i, ii, ip, ib;

// --- Global Variables ---

/* Helper variable for handling key press */
UBYTE key_down;

/* If 1 a dialog window is currently on screen */
UBYTE dia_wnd_visible;

/* Amount of missing lines to display */
UINT8 open_dia_lines;

// --- Functions ---

/* Simple replacement for wait() function */
void performant_delay(UINT8 numloops)
{
    for (ip = 0; i != numloops; i++)
    {
        wait_vbl_done();
    }
}

/* Draws given text in a black box on the window layer
    - text_lines -> Mulit-Array with text lines
    - amount_lin -> The amount of lines which were already displayed
    - last_lin -> The last line which was displayed
    --> return value -> The amount of lines which have to be displayed
*/
UINT8 show_dia_wnd(unsigned char text_lines[][MAX_STRING_SIZE], UINT8 amount_lin, UINT8 last_lin)
{
    dia_wnd_visible = 1;

    set_win_data(blk_tile_adr, 1, black_tile);
    set_win_data(arr_tile_adr, 1, arrow_tile);

    // Clear Window with black
    for (ib = 0; ib != 32; ib++)
    {
        set_win_data(dia_first_row_start_adr + ib, 1, black_tile);
    }

    // Write the next two lines
    for (i = 0; i != 2; i++)
    {
        for (ii = 0; ii < 16; ii++)
        {
            if (i == 0)
            {
                set_win_data(dia_first_row_start_adr + ii, 1, char_sprites[text_lines[i + last_lin][ii] - char_offset]);
            }
            else
            {
                set_win_data(0xe0 + ii, 1, char_sprites[text_lines[i + last_lin][ii] - char_offset]);
            }
        }

        // When amount of lines is even
        if ((i + 1) + last_lin == amount_lin)
        {
            return 0;
        }

        // When amount of lines is odd
        if (i + last_lin == amount_lin)
        {
            // remove second, black line
            for (ib = 0; ib != 16; ib++)
            {
                set_win_data(dia_second_row_start_adr + ib, 1, black_tile);
            }
            return 0;
        }
    }

    set_win_tiles(0, 0, 18, 5, dia_wnd_tilemap);
    move_win(15, 96);

    SHOW_WIN;

    return i + last_lin;
}

// --- Entry Point ---

void main()
{
    // Fill sprite data with noise
    set_sprite_data(0, 256, noise_data);
    set_bkg_data(0, 128, noise_data);

    DISPLAY_ON;
    SHOW_BKG;

    open_dia_lines = 0;

    while (1)
    {
        if(key_down)
        {
            waitpadup();
            key_down = 0;
        }

        switch (joypad())
        {
            case J_A:
                // Continue text if visible and possible - else close dialog
                if (dia_wnd_visible == 1 & open_dia_lines == 0)
                {
                    HIDE_WIN;
                    dia_wnd_visible = 0;
                    open_dia_lines = 0;
                }
                else
                {
                    open_dia_lines = show_dia_wnd(pangram_a, sizeof(pangram_a) / MAX_STRING_SIZE, open_dia_lines);
                }

                key_down = 1;
                break;

            case J_B:
                // Close dialog in any case if visible
                if (dia_wnd_visible == 1)
                {
                    HIDE_WIN;
                    dia_wnd_visible = 0;
                    open_dia_lines = 0;
                }

                key_down = 1;
                break;
        }

        performant_delay(5);
    }
}