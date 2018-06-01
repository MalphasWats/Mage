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

void display_window(point top_left, byte width, byte height)
{
    byte b = 0;
    for(byte y=0 ; y<height ; y++)
    {
        for(byte x=0 ; x<width ; x++)
        {
            if(x==0 && y==0)
                b=245;
            else if(x==width-1 && y==0)
                b=246;
            else if(x==0 && y==height-1)
                b=247;
            else if(x==width-1 && y==height-1)
                b=248;
            else if(y==0 || y==height-1)
                b=249;
            else if(x==0 || x==width-1)
                b=250;
            else
                b=0; // space!
            
            display_block(&GLYPHS[b*8], x+top_left.x, y+top_left.y);
                
        }
    }
}

void battle_mode(mob_type *player, mob_type *opponent)
{
    byte in_battle = TRUE;
    btn_timer = 0;
    
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
    display_block(&GLYPHS[9*8], 5+player->num_actions+1, 4);
    
    //TODO: Need to be cleverer about lining these up
    display_block(&GLYPHS[8*8], 5, 2);
    display_block(&GLYPHS[9*8], 5+opponent->num_actions+1, 2);
    
    //draw player TODO: scale x2
    //http://tech-algorithm.com/articles/nearest-neighbor-image-scaling/
    /*byte glyph[8];
    for (int i=0 ; i<8 ; i++)
        glyph[i] = pgm_read_byte(&GLYPHS[player->glyph]+i);
    
    unsigned int buffer[8*2];
    unsigned long x_ratio = 32769;//(unsigned long)((8<<16)/16) +1;
    unsigned long y_ratio = 32769;//(unsigned long)((8<<16)/16) +1;
    
    unsigned int x2;
    unsigned int y2;
    for (unsigned int i=0;i<16;i++) 
    {
        for (unsigned int j=0;j<16;j++) 
        {
            x2 = (unsigned int)((j*x_ratio)>>16) ;
            y2 = (unsigned int)((i*y_ratio)>>16) ;
            if (glyph[y2] & (1 << x2))
                buffer[i] |= 1 << j;
        }                
    }
    
    set_display_col_row(1, 6);
    for (byte i=0 ; i<16 ; i++)
    {
        shift_out((byte)buffer[i] >> 8, LSBFIRST);
    }
    
    set_display_col_row(1, 7);
    for (byte i=0 ; i<16 ; i++)
    {
        shift_out((byte)buffer[i] & 0x0f, LSBFIRST);
    }*/
    display_block(&GLYPHS[player->glyph*8], 1, 6);
    
    //draw opponent TODO: scale x2
    display_block(&GLYPHS[opponent->glyph*8], 13, 2);

    byte turn=0;
    while(in_battle)
    {
        //update countdown
        
        /* Display player health */
        for(byte i=0 ; i<((player->hitpoints & 0xf0) >> 4) ; i++)
        {
            if( i >= (player->hitpoints & 0x0f) )
                display_block(&GLYPHS[254*8], 0+i, 5);
            else
                display_block(&GLYPHS[255*8], 0+i, 5);
        }
        
        /* Display opponent health */
        for(byte i=0 ; i<((opponent->hitpoints & 0xf0) >> 4) ; i++)
        {
            if( i >= (opponent->hitpoints & 0x0f) )
                display_block(&GLYPHS[254*8], 15-i, 1);
            else
                display_block(&GLYPHS[255*8], 15-i, 1);
        }

        byte cursor = 0;
        word player_actions = 0;
        word opponent_actions = 0;
        
        byte p_action = 0;
        
        //choose opponent action(s)
        for(byte i=0 ; i<opponent->num_actions ; i++)
        {
            opponent_actions |= ((opponent->tactics>>((turn%4)*2)) & 0x03) << (i*2);
            display_block(&GLYPHS[238*8], 6+i, 2);
        }
        
        // Clear player actions
        for(byte i=0 ; i<player->num_actions ; i++)
            display_block(&GLYPHS[0], 6+i, 4);
        
        byte player_turn = TRUE;
        //await player actions
        while(player_turn)
        {
            t = millis();
            btn_val = analog_read(ADC2);
            if (btn_timer == 0)
            {
                if (btn_val >= _LEFT-ADC_VAR && btn_val <= _LEFT+ADC_VAR)
                {
                    //display_block(&GLYPHS[0], cursor.x, cursor.y);
                    cursor -= 1;
                    crap_beep(_C5, 5);
                    btn_timer = t;
                }
                else if (btn_val >= _RIGHT-ADC_VAR && btn_val <= _RIGHT+ADC_VAR)
                {
                    //display_block(&GLYPHS[0], cursor.x, cursor.y);
                    cursor += 1;
                    crap_beep(_C5, 5);
                    btn_timer = t;
                }
                else if (btn_val >= _A-ADC_VAR && btn_val <= _A+ADC_VAR)
                {
                    crap_beep(_C5, 5);
                    
                    btn_timer = t;
                    
                    if (cursor == 0)
                    {
                        display_block(&GLYPHS[239*8], p_action+6, 4);
                        player_actions |= 0b10 << (p_action*2);
                        p_action += 1;
                    }
                    else if (cursor == 1)
                    {
                        display_block(&GLYPHS[240*8], p_action+6, 4);
                        player_actions |= 0b01 << (p_action*2);
                        p_action += 1;
                    }
                    else if (cursor == 2)
                    {
                        display_block(&GLYPHS[241*8], p_action+6, 4);
                        player_actions |= 0b11 << (p_action*2);
                        p_action += 1;
                    }
                    
                    if (p_action == player->num_actions)
                        player_turn = FALSE;
                }
                else if (btn_val >= _C-ADC_VAR && btn_val <= _C+ADC_VAR)
                {
                    crap_beep(_Gs5, 20);
                    
                    btn_timer = t;
                    
                    // End battle mode for now
                    opponent->dead = TRUE;
                    in_battle = FALSE;
                    player_turn = FALSE;
                }
            }
            
            if (t - btn_timer > BTN_DELAY)
                btn_timer = 0;
            
            if (cursor == 255)
                cursor = 2;
            if (cursor > 2)
                cursor = 0;
            
            if (p_action < player->num_actions)
                display_block(&GLYPHS[237*8], 6+p_action, 4);
            
            //display_block(&GLYPHS[59*8], cursor.x, cursor.y);
            for (byte i=0 ; i<3 ; i++)
            {
                //draw available actions
                if (i == cursor)
                    display_block(&GLYPHS[(239+i+3)*8], 4+i, 6);
                else
                    display_block(&GLYPHS[(239+i)*8], 4+i, 6);
            }
            
        }
        
        //resolve combat
        crap_beep(_A4, 20);
        delay_ms(10);
        crap_beep(_A5, 35);
        delay_ms(30);
        crap_beep(_B5, 20);
        delay_ms(10);
        crap_beep(_C5, 35);
             
        byte p_attack_mod = 0;
        byte p_defence_mod = 0;
        byte o_attack_mod = 0;
        byte o_defence_mod = 0;
        
        for(byte i=0 ; i<4 ; i++)
        {
            byte pa = (player_actions >> (i*2)) & 0x0003;
            if (pa == 1)
                p_defence_mod += 3;
            if (pa == 3)
                p_attack_mod += 3;
            
            
            byte oa = (opponent_actions >> (i*2)) & 0x0003;
            if (oa == 1)
            {
                o_defence_mod += 3;
                display_block(&GLYPHS[240*8], 6+i, 2);
            }
            if (oa == 3)
            {
                o_attack_mod += 3;
                display_block(&GLYPHS[241*8], 6+i, 2);
            }
            
            if (pa == 2)
            {
                //int roll = (rng() % 20) + player->attack + p_attack_mod;
                if ( (rng() % 20) + player->attack + p_attack_mod > opponent->defence + o_defence_mod)
                //if ( roll > opponent->defence + o_defence_mod)
                {
                    if (player->damage >= (opponent->hitpoints & 0x0f))
                    {
                        opponent->hitpoints = opponent->hitpoints & 0xf0;
                        opponent->dead = TRUE;
                        opponent->glyph = 91;
                    }
                    else
                    {
                        opponent->hitpoints -= player->damage;
                    }
                }
            }
            
            if (oa == 2)
            {
                display_block(&GLYPHS[239*8], 6+i, 2);
                if ( (rng() % 20) + opponent->attack + o_attack_mod > player->defence + p_defence_mod)
                {
                    if (opponent->damage >= (player->hitpoints & 0x0f))
                    {
                        player->hitpoints = player->hitpoints & 0xf0;
                        player->dead = TRUE;
                        player->glyph = 91;
                    }
                    else
                        player->hitpoints -= opponent->damage;
                    
                }
            }
            
            if(opponent->dead || player->dead)
            {
                in_battle = FALSE;
                break;
            }
        }
            
        //show result
        delay_ms(200);
        
        turn += 1;
        p_action = 0;
        //opponent_actions = 0;
    }
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
        .position = {.x=10, .y=8},
    
        .hitpoints = (4<<4) | 4,
        .damage = 1,
        .attack = 0,
        .defence = 5,
        .num_actions = 1,
        
        .tactics = 0b10101010, // blobs just attack
    
        .dead = FALSE,
    };
    
    current_location->mobs[0] = &mob;
    
    mob_type player = {
        .glyph = PLAYER_OFFSET,
        .position = current_location->portal_in,
    
        .attack = 1,
        .defence = 6,
        .hitpoints = (3<<4) | 3,
        .damage = 2,
        .num_actions = 2,
        
        .tactics = 0, // Not applicable to player
    
        .dead = FALSE,
    };
    
    for(ever)
    {
        t = millis();
        
        btn_val = analog_read(ADC2);
        
        //display_hud(btn_val); // Debug buttons
        if (btn_timer == 0)
        {
            rng(); // cycle the rng to try to get different values
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
                
                display_window((point){2, 2}, 5, 4);
                
                while(analog_read(ADC2) > 50){}
                //btn_timer = t;
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
        if (player.dead)
        {
            //display something sad
            for(ever) {}
        }
        
        mob_type *opponent = update_mobs(current_location, &player);
        
        if (opponent != 0 && !opponent->dead)
        {
            crap_beep(_A4, 20);
            delay_ms(10);
            crap_beep(_A5, 35);
            battle_mode(&player, opponent);
            
            map_dirty = TRUE;
        }
        
        display_mobs(current_location);
        //delta = millis() - t;
        //display_hud(delta);
    }
}