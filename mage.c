#include <avr/io.h>

#include "mage.h"
#include "oled.h"
#include "glyphs.h"
#include "beep.h"

//byte HUD[4] = {0x00, 0x00, 0x00, 0x00};
unsigned int btn_timer = 0;
int btn_val = 0;

unsigned int t;

int viewport_col = 0;
int viewport_row = 0;

void display_map(location *loc)
{   
    int col, row, i;
    
    for (row=0 ; row<SCREEN_ROWS ; row++)
    {
        set_display_row(row);
        for (col=0 ; col<SCREEN_COLUMNS ; col++)
        {
            i = loc->width * (viewport_row+row) + (viewport_col+col);
            shift_out_block(&GLYPHS[pgm_read_byte(&loc->map[ i ])*8]);
        }
    }
}

void display_hud(unsigned int value)
{
    for (byte d=0 ; d<4 ; d++)    // Button Mapping
    {
        display_block( &GLYPHS[((value % 10)+DIGIT_OFFSET)*8], 4-d, 0);
        value = value / 10;
    }
    
    delay_ms(50);
}

void display_player(mob_type *player)
{
    display_block(&GLYPHS[player->glyph*8], player->position.x-viewport_col, (player->position.y-viewport_row));
}

mob_type *update_mobs(location *loc, mob_type *player)
{
    for(byte i=0 ; i<MAX_MOBS ; i++)
    {
        if (loc->mobs[i])
        {
            if (loc->mobs[i]->position.x == player->position.x && 
                loc->mobs[i]->position.y == player->position.y &&
                !loc->mobs[i]->dead)
            {
                return loc->mobs[i];
            }
            // update the mob
        }
    }
    return 0;
}

void display_mobs(location *loc)
{
    for(byte i=0 ; i<MAX_MOBS ; i++)
    {
        if (loc->mobs[i])
        {
            if (loc->mobs[i]->position.x >= viewport_col && loc->mobs[i]->position.x < viewport_col+SCREEN_COLUMNS && 
                loc->mobs[i]->position.y >= viewport_row && loc->mobs[i]->position.y < viewport_row+SCREEN_ROWS )
            {
                display_block(&GLYPHS[loc->mobs[i]->glyph*8], loc->mobs[i]->position.x-viewport_col, (loc->mobs[i]->position.y-viewport_row));
            }
        }
    }
}

void battle_mode(mob_type *player, mob_type *opponent)
{
    byte in_battle = TRUE;
    btn_timer = 0;
    
    t = millis();
    //clear screen
    clear_display();
    
    crap_beep(_A4, 20);
    delay_ms(10);
    crap_beep(_A5, 35);
    delay_ms(30);
    crap_beep(_B5, 20);
    delay_ms(10);
    crap_beep(_C5, 35);
    
    
    //draw ui
    display_block(&GLYPHS[32*8], 1, 1);
    display_block(&GLYPHS[DIGIT_OFFSET*8], 2, 1);
    display_block(&GLYPHS[DIGIT_OFFSET*8], 3, 1);
    
    display_block(&GLYPHS[8*8], 5, 4);
    display_block(&GLYPHS[9*8], 5+player->num_attacks+1, 4);
    
    //TODO: Need to be cleverer about lining these up
    display_block(&GLYPHS[8*8], 5+player->num_attacks-opponent->num_attacks, 2);
    display_block(&GLYPHS[9*8], 5+player->num_attacks+1, 2);
    
    //draw player TODO: scale x2
    //http://tech-algorithm.com/articles/nearest-neighbor-image-scaling/
    /*byte glyph[8];
    for (int i=0 ; i<8 ; i++)
        glyph[i] = pgm_read_byte(&GLYPHS[player->glyph]+i);
    
    unsigned int buffer[8*2];
    unsigned long x_ratio = 32769;//(unsigned long)((8<<16)/16) +1;
    unsigned long y_ratio = 32769;//(unsigned long)((8<<16)/16) +1;
    
    int x2, y2 ;
    for (int i=0;i<16;i++) 
    {
        for (int j=0;j<16;j++) 
        {
            x2 = ((j*x_ratio)>>16) ;
            y2 = ((i*y_ratio)>>16) ;
            if (glyph[y2] & (1 << x2))
                buffer[i] |= 1 << j;
        }                
    }
    
    set_display_col_row(1, 6);
    for (int i=0 ; i<16 ; i++)
    {
        shift_out((byte)buffer[i], LSBFIRST);
    }
    
    set_display_col_row(1, 7);
    for (int i=0 ; i<16 ; i++)
    {
        shift_out((byte)buffer[i+16], LSBFIRST);
    }*/
    
    display_block(&GLYPHS[player->glyph*8], 1, 6);
    for(byte i=0 ; i<player->hitpoints/2 ; i++)
    {
        display_block(&GLYPHS[255*8], 0+i, 5);
    }
    if (player->hitpoints%2)
    {
        display_block(&GLYPHS[253*8], 0+(player->hitpoints/2), 5);
    }
    
    //draw opponent TODO: scale x2
    display_block(&GLYPHS[opponent->glyph*8], 13, 2);
    for(byte i=0 ; i<opponent->hitpoints/2 ; i++)
    {
        display_block(&GLYPHS[255*8], 15-i, 1);
    }
    if (opponent->hitpoints%2)
    {
        display_block(&GLYPHS[254*8], 15-(opponent->hitpoints/2), 1);
    }
    
    //start countdown
    
    //choose opponent action(s)
    
    //await player actions
    while(in_battle)
    {
        btn_val = analog_read(ADC2);
        if (btn_val >= _C-ADC_VAR && btn_val <= _C+ADC_VAR && btn_timer == 0)
        {
            crap_beep(_Gs5, 20);
            
            btn_timer = t;
            
            // End battle mode for now
            opponent->dead = TRUE;
            in_battle=FALSE;
        }
        
        if (t - btn_timer > BTN_DELAY)
            btn_timer = 0;

    }
    //resolve combat
    
    //update ui
}

byte collide_at(location *loc, byte col, byte row)
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
    crap_beep(_A5, 140);
    delay_ms(5);
    crap_beep(_A8, 60);
    
    delay_ms(SPLASH_DELAY);
    
    PORTB &= ~(1 << DC);        // LOW
    send_command(0xA7);        // INVERTEDDISPLAY
    PORTB |= 1 << DC;           // HIGH
    
    //unsigned int delta = 0;
    //unsigned int hud_timer = 0;
    
    byte map_dirty = TRUE;
    
    build_location_portals();
    location *current_location = &village;
    
    mob_type mob = {
        .glyph = (PLAYER_OFFSET+7),   // Blob
        .position = {.x=16, .y=13},
    
        .hitpoints = 2,
        .attack_damage = 1,
        .num_attacks = 1,
        
        .tactics = 0b10101010, // blobs just attack
    
        .dead = FALSE,
    };
    
    current_location->mobs[0] = &mob;
    
    mob_type player = {
        .glyph = PLAYER_OFFSET,
        .position = current_location->portal_in,
    
        .hitpoints = 5,
        .attack_damage = 2,
        .num_attacks = 2,
        
        .tactics = 0, // Not applicable to player
    
        .dead = FALSE,
    };
    
    for(ever)
    {
        t = millis();
        
        btn_val = analog_read(ADC2);
        
        //display_hud(btn_val);
        if (btn_timer == 0)
        {
            if (btn_val >= _LEFT-ADC_VAR && btn_val <= _LEFT+ADC_VAR)
            {
                if (!collide_at(current_location, player.position.x-1, player.position.y))
                {
                    player.position.x -= 1;
                    map_dirty = TRUE;
                    crap_beep(_A9, 5);
                }
                
                btn_timer = t;
            }
            else if (btn_val >= _RIGHT-ADC_VAR && btn_val <= _RIGHT+ADC_VAR)
            {
                if (!collide_at(current_location, player.position.x+1, player.position.y))
                {
                    player.position.x += 1;
                    map_dirty = TRUE;
                    crap_beep(_A9, 5);
                }
                btn_timer = t;
            }
            else if (btn_val >= _UP-ADC_VAR && btn_val <= _UP+ADC_VAR)
            {
                if (!collide_at(current_location, player.position.x, player.position.y-1))
                {
                    player.position.y -= 1;
                    map_dirty = TRUE;
                    crap_beep(_A9, 5);
                }
                
                btn_timer = t;
            }
            else if (btn_val >= _DOWN-ADC_VAR && btn_val <= _DOWN+ADC_VAR)
            {
                if (!collide_at(current_location, player.position.x, player.position.y+1))
                {
                    player.position.y += 1;
                    map_dirty = TRUE;
                    crap_beep(_A9, 5);
                }
                
                btn_timer = t;
            }
            else if (btn_val >= _A-ADC_VAR && btn_val <= _A+ADC_VAR)
            {
                if (player.position.x == current_location->portal_out.x && 
                    player.position.y == current_location->portal_out.y &&
                    current_location->return_to != 0)
                {
                    player.position = current_location->portal_in;
                    current_location = current_location->return_to;
                    map_dirty = TRUE;
                    crap_beep(_A9, 5);
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
                            crap_beep(_A9, 5);
                            
                            break;
                        }
                    }
                }
                
                btn_timer = t;
            }
            else if (btn_val >= _B-ADC_VAR && btn_val <= _B+ADC_VAR)
            {
                map_dirty = TRUE;
                
                crap_beep(_A9, 5);
                
                btn_timer = t;
            }
            else if (btn_val >= _C-ADC_VAR && btn_val <= _C+ADC_VAR)
            {
                map_dirty = TRUE;
                    
                crap_beep(_A9, 5);
                
                btn_timer = t;
            }
            else if (btn_val >= _D)
            {
                map_dirty = TRUE;
                    
                crap_beep(_A9, 5);
                
                btn_timer = t;
            }
        }
        
        if (t - btn_timer > BTN_DELAY)
            btn_timer = 0;
        
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
        
        display_player(&player);
        
        mob_type *opponent = update_mobs(current_location, &player);
        
        if (opponent != 0)
        {
            crap_beep(_A4, 20);
            delay_ms(10);
            crap_beep(_A5, 35);
            battle_mode(&player, opponent);
        }
        
        display_mobs(current_location);
        
        //delta = millis() - t;
        //display_hud(delta);
    }
}