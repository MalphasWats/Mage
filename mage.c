#include <avr/io.h>

#include "mage.h"
#include "oled.h"
#include "glyphs.h"
#include "maps.h"
#include "beep.h"

byte HUD[4] = {0x00, 0x00, 0x00, 0x00};
unsigned int btn_timers[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int viewport_col = 0;
int viewport_row = 0;
        
location build_locations()
{
    location village = { 
        .portal_in={15, 14}, 
        .portal_out={0, 0}, 
        .map=&VILLAGE[0],
        .width=32,
        .height=16,
        .player={15, 14}, 
        .return_to=0 
    };
    location loc = { 
        .portal_in={8, 7}, 
        .portal_out={11, 10}, 
        .map=&HOUSE[0],
        .width=8,
        .height=8,
        .player={0, 0}, 
        .return_to=&village
    };
    
    village.portals[0] = &loc;
    
    return village;
}

void display_map(location *loc)
{
    viewport_col = loc->player.x-SCREEN_COLUMNS/2;
    viewport_row = loc->player.y-SCREEN_ROWS/2;
    
    if (viewport_col < 0)
        viewport_col = 0;
    if (viewport_row < 0)
        viewport_row = 0;
    
    if (viewport_col + SCREEN_COLUMNS > loc->width)
        viewport_col = loc->width - SCREEN_COLUMNS;
    if (viewport_row + SCREEN_ROWS > loc->height)
        viewport_row = loc->height - SCREEN_ROWS;
    
    int col, row, i;
    
    for (row=0 ; row<SCREEN_ROWS ; row++)
    {
        for (col=0 ; col<SCREEN_COLUMNS ; col++)
        {
            i = loc->width * (viewport_row+row) + (viewport_col+col);
            if (i > loc->width * loc->height)
                shift_out_block(&GLYPHS[0]);
            else
                shift_out_block(&GLYPHS[pgm_read_byte(&loc->map[ i ])*8]);
        }
    }
    
    // Drawing sections of screen
    // https://www.ccsinfo.com/forum/viewtopic.php?p=217165
}

void display_player(location *loc)
{
    display_block(&GLYPHS[PLAYER_OFFSET*8], (loc->player.x-viewport_col)*8, (loc->player.y-viewport_row));
}



byte collide_at(location *loc, int col, int row)
{
    // odd collide, even passable, TODO: above ~128
    return pgm_read_byte(&loc->map[ loc->width * row + col ]) % 2;
}

int main (void) 
{
    // Setup
    //DDRB = (1<<SDA) | (1<<DC) | (1<<SCL) | (1<<SND); // Configure Outputs
    
    init_timer();
    
    initialise_oled();
    clear_display();
    
    PORTB |= 1 << DC;           // HIGH
    
    display_image(&LOGO[0], 3, 3, 10, 2);
    crap_beep(SND, _A5, 140);
    delay_ms(5);
    crap_beep(SND, _A8, 60);
    
    delay_ms(SPLASH_DELAY);
    
    PORTB &= ~(1 << DC);        // LOW
    send_command(0xA7);        // INVERTEDDISPLAY
    PORTB |= 1 << DC;           // HIGH
    
    unsigned int t;
    //unsigned int delta = 0;
    unsigned int btn_val = 0;
    //unsigned int hud_timer = 0;
    
    byte map_dirty = TRUE;
    
    location current_location = build_locations();
    
    for(ever)
    {
        t = millis();
        
        if (map_dirty)
        {
            //display_map(&MAP1[0], MAP_1_COLS, MAP_1_ROWS);
            display_map(&current_location);
            map_dirty = FALSE;
        }
        
        /*if (hud_timer == 0)
        {
            if (btn_val > 50)
            {
                delta = btn_val;
                for (byte d=0 ; d<4 ; d++)    // Button Mapping
                {
                    HUD[3-d] = (delta % 10);
                    delta = delta / 10;
                }
            }
            else 
            {
                for (byte d=0 ; d<3 ; d++)      // Frame Time
                {
                    HUD[3-d] = (delta % 10);
                    delta = delta / 10;
                }
                HUD[0] = 0;
            }
            
            hud_timer = t;
        }
        if (t - hud_timer > 100)
            hud_timer = 0;
        */
            
        btn_val = analog_read(ADC2);
        
        if (btn_val >= _LEFT-ADC_VAR && btn_val <= _LEFT+ADC_VAR && btn_timers[0] == 0)
        {
            if (!collide_at(&current_location, current_location.player.x-1, current_location.player.y))
            {
                current_location.player.x -= 1;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            
            btn_timers[0] = t;
        }
        else if (btn_val >= _RIGHT-ADC_VAR && btn_val <= _RIGHT+ADC_VAR && btn_timers[1] == 0)
        {
            if (!collide_at(&current_location, current_location.player.x+1, current_location.player.y))
            {
                current_location.player.x += 1;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            btn_timers[1] = t;
        }
        else if (btn_val >= _UP-ADC_VAR && btn_val <= _UP+ADC_VAR && btn_timers[2] == 0)
        {
            if (!collide_at(&current_location, current_location.player.x, current_location.player.y-1))
            {
                current_location.player.y -= 1;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            
            btn_timers[2] = t;
        }
        else if (btn_val >= _DOWN-ADC_VAR && btn_val <= _DOWN+ADC_VAR && btn_timers[3] == 0)
        {
            if (!collide_at(&current_location, current_location.player.x, current_location.player.y+1))
            {
                current_location.player.y += 1;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            
            btn_timers[3] = t;
        }
        else if (btn_val >= _A-ADC_VAR && btn_val <= _A+ADC_VAR && btn_timers[4] == 0)
        {
            map_dirty = TRUE;
            
            crap_beep(SND, _A9, 5);
            
            btn_timers[4] = t;
        }
        else if (btn_val >= _B-ADC_VAR && btn_val <= _B+ADC_VAR && btn_timers[5] == 0)
        {
            map_dirty = TRUE;
            
            crap_beep(SND, _A9, 5);
            
            btn_timers[5] = t;
        }
        else if (btn_val >= _C-ADC_VAR && btn_val <= _C+ADC_VAR && btn_timers[6] == 0)
        {
            map_dirty = TRUE;
                
            crap_beep(SND, _A9, 5);
            
            btn_timers[6] = t;
        }
        else if (btn_val >= _D && btn_timers[7] == 0)
        {
            map_dirty = TRUE;
                
            crap_beep(SND, _A9, 5);
            
            btn_timers[7] = t;
        }
        
        if (t - btn_timers[0] > BTN_DELAY)
            btn_timers[0] = 0;
        if (t - btn_timers[1] > BTN_DELAY)
            btn_timers[1] = 0;
        if (t - btn_timers[2] > BTN_DELAY)
            btn_timers[2] = 0;
        if (t - btn_timers[3] > BTN_DELAY)
            btn_timers[3] = 0;
        if (t - btn_timers[4] > BTN_DELAY)
            btn_timers[4] = 0;
        if (t - btn_timers[5] > BTN_DELAY)
            btn_timers[5] = 0;
        if (t - btn_timers[6] > BTN_DELAY)
            btn_timers[6] = 0;
        if (t - btn_timers[7] > BTN_DELAY)
            btn_timers[7] = 0;
        
        if (current_location.player.x < 0)
            current_location.player.x = 0;
        if (current_location.player.x > current_location.width-1)
            current_location.player.x = current_location.width-1;
        
        if (current_location.player.y < 0)
            current_location.player.y = 0;
        if (current_location.player.y > current_location.height-1)
            current_location.player.y = current_location.height-1;
        
        //display_hud();
        display_player(&current_location);
        
        //delta = millis() - t;
    }
}

/*

avr-gcc -c -mmcu=atmega16 file1.c -o file1.o
avr-gcc -c -mmcu=atmega16 file2.c -o file2.o
avr-gcc -c -mmcu=atmega16 file3.c -o file3.o
etc.

which separately compiles (-c) three different files to generate ELF object files. Then you join those together using the linker:

avr-gcc -mmcu=atmega16 file1.o file2.o file3.o -o proj.elf

The absence of -c means "don't just compile but link together the inputs". So cross-references in the files will be linked to each other and this will finally write a fully linked copy of the code to the proj.elf output file. Normally you then want to extract .text and .data to make a .hex from this with:

avr-objcopy -O ihex -j .text -j .data proj.elf proj.hex

*/