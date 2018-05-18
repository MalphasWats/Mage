#include <avr/io.h>

#include "mage.h"
#include "oled.h"
#include "glyphs.h"
#include "beep.h"

byte HUD[4] = {0x00, 0x00, 0x00, 0x00};
unsigned int btn_timers[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int viewport_col = 0;
int viewport_row = 0;

void display_map(location *loc)
{   
    int col, row, i;
    
    for (row=0 ; row<SCREEN_ROWS ; row++)
    {
        for (col=0 ; col<SCREEN_COLUMNS ; col++)
        {
            i = loc->width * (viewport_row+row) + (viewport_col+col);
            shift_out_block(&GLYPHS[pgm_read_byte(&loc->map[ i ])*8]);
        }
    }
    
    // Drawing sections of screen
    // https://www.ccsinfo.com/forum/viewtopic.php?p=217165
}

void display_player(mob_type *p)
{
    display_block(&GLYPHS[p->glyph], (p->position.x-viewport_col)*8, (p->position.y-viewport_row));
}



byte collide_at(location *loc, int col, int row)
{
    // odd collide, even passable, TODO: above 128
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
    
    build_location_portals();
    
    location *current_location = &village;
    
    mob_type player = {
        .glyph = PLAYER_OFFSET*8,
        .position = current_location->portal_in,
    
        .hitpoints = 10,    
        .attack_damage = 2,
        .num_attacks = 2,
    
        .dead = FALSE,
    };
    
    for(ever)
    {
        t = millis();
            
        btn_val = analog_read(ADC2);
        
        if (btn_val >= _LEFT-ADC_VAR && btn_val <= _LEFT+ADC_VAR && btn_timers[0] == 0)
        {
            if (!collide_at(current_location, player.position.x-1, player.position.y))
            {
                player.position.x -= 1;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            
            btn_timers[0] = t;
        }
        else if (btn_val >= _RIGHT-ADC_VAR && btn_val <= _RIGHT+ADC_VAR && btn_timers[1] == 0)
        {
            if (!collide_at(current_location, player.position.x+1, player.position.y))
            {
                player.position.x += 1;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            btn_timers[1] = t;
        }
        else if (btn_val >= _UP-ADC_VAR && btn_val <= _UP+ADC_VAR && btn_timers[2] == 0)
        {
            if (!collide_at(current_location, player.position.x, player.position.y-1))
            {
                player.position.y -= 1;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            
            btn_timers[2] = t;
        }
        else if (btn_val >= _DOWN-ADC_VAR && btn_val <= _DOWN+ADC_VAR && btn_timers[3] == 0)
        {
            if (!collide_at(current_location, player.position.x, player.position.y+1))
            {
                player.position.y += 1;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            
            btn_timers[3] = t;
        }
        else if (btn_val >= _A-ADC_VAR && btn_val <= _A+ADC_VAR && btn_timers[4] == 0)
        {
            if (player.position.x == current_location->portal_out.x && 
                player.position.y == current_location->portal_out.y &&
                current_location->return_to != 0)
            {
                current_location = current_location->return_to;
                player.position = current_location->portal_in;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            else 
            {
                for (byte i=0 ; i<MAX_PORTALS ; i++)
                {
                    if (player.position.x == current_location->portals[i]->portal_in.x && 
                        player.position.y == current_location->portals[i]->portal_in.y)
                    {
                        current_location = current_location->portals[i];
                        player.position.x = current_location->portal_out.x;
                        player.position.y = current_location->portal_out.y;
                        
                        map_dirty = TRUE;
                        crap_beep(SND, _A9, 5);
                        
                        break;
                    }
                }
            }
            
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
        
        if (player.position.x < 0)
            player.position.x = 0;
        if (player.position.x > current_location->width-1)
            player.position.x = current_location->width-1;
        
        if (player.position.y < 0)
            player.position.y = 0;
        if (player.position.y > current_location->height-1)
            player.position.y = current_location->height-1;
        
        if (map_dirty)
        {
            viewport_col = player.position.x-SCREEN_COLUMNS/2;
            viewport_row = player.position.y-SCREEN_ROWS/2;
            
            if (viewport_col < 0)
                viewport_col = 0;
            if (viewport_row < 0)
                viewport_row = 0;
            
            if (viewport_col + SCREEN_COLUMNS > current_location->width)
                viewport_col = current_location->width - SCREEN_COLUMNS;
            if (viewport_row + SCREEN_ROWS > current_location->height)
                viewport_row = current_location->height - SCREEN_ROWS;
    
            display_map(current_location);
            map_dirty = FALSE;
        }
        
        //display_hud();
        display_player(&player);
        
        //delta = millis() - t;
    }
}