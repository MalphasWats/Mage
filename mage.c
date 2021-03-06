#include <avr/io.h>

#include "mage.h"
#include "oled.h"
#include "glyphs.h"
#include "beep.h"

unsigned int btn_timer = 0;
int btn_val = 0;

unsigned int t;
unsigned int spawn_timer;

int viewport_col = 0;
int viewport_row = 0;

byte cursor = 0;

item combat_actions[] = {
	(item){.glyph=98, .attributes=0},
	(item){.glyph=99, .attributes=0},
	(item){.glyph=100, .attributes=0},
};

item inventory[PLAYER_INVENTORY];

void display_map(location *loc)
{   
    int col, row, i;
    
    for (row=0 ; row<SCREEN_ROWS ; row++)
    {
        set_display_col_row(0, row);
        for (col=0 ; col<SCREEN_COLUMNS ; col++)
        {
            i = loc->width * (viewport_row+row) + (viewport_col+col);
            shift_out_block(&GLYPHS[pgm_read_byte(&loc->map[ i ])*8], FALSE);
        }
    }
}

/*void display_hud(unsigned int value)
{
    for (byte d=0 ; d<4 ; d++)    // Button Mapping
    {
        display_block( &GLYPHS[((value % 10)+DIGIT_OFFSET)*8], 4-d, 0);
        value = value / 10;
    }
    
    delay_ms(50);
}*/

/*void display_player(mob_type *player)
{
    display_block(&GLYPHS[player->glyph*8], player->position.x-viewport_col, (player->position.y-viewport_row));
}*/

/*mob_type *update_mobs(location *loc, mob_type *player)
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
            //TODO: update the mob
        }
    }
    return 0;
}*/

/*void display_mobs(location *loc)
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
}*/

void display_window(point top_left, byte width, byte height)
{
    byte b = 0;
    for(byte y=0 ; y<height ; y++)
    {
        for(byte x=0 ; x<width ; x++)
        {
            if(x==0 && y==0)
                b=106;
            else if(x==width-1 && y==0)
                b=107;
            else if(x==0 && y==height-1)
                b=108;
            else if(x==width-1 && y==height-1)
                b=109;
            else if(y==0 || y==height-1)
                b=110;
            else if(x==0 || x==width-1)
                b=111;
            else
                b=0; // space!
            
            display_block(&GLYPHS[b*8], x+top_left.x, y+top_left.y);
        }
    }
}

/*void display_string(const char *str, byte col, byte row)
{
    char buffer[16];
    strcpy_P(buffer, str);
    for(byte i=0 ; buffer[i] != '\0' ; i++)
    {
        display_block(&GLYPHS[(buffer[i]-32)*8], col+i, row);
    }
}*/

byte display_item_window(point top_left, item *items, byte num_items, byte width)
{
    byte height = num_items / width;
    if (num_items % width > 0)
        height += 1;
    
    //display_window((point){5, 4-h}, 6, h+2);
    byte col = 0;
    byte row = 0;
    
    //char cursor = 0;
    
    for(ever)
    {
        t = millis();
        btn_val = analog_read(ADC2);
        if (btn_timer == 0)
        {
            if (btn_val >= _LEFT-ADC_VAR && btn_val <= _LEFT+ADC_VAR)
            {
                cursor -= 1;
                click();
                btn_timer = t;
            }
            else if (btn_val >= _RIGHT-ADC_VAR && btn_val <= _RIGHT+ADC_VAR)
            {
                cursor += 1;
                click();
                btn_timer = t;
            }
            else if (btn_val >= _UP-ADC_VAR && btn_val <= _UP+ADC_VAR)
            {
                cursor -= 4;
                click();
                btn_timer = t;
            }
            else if (btn_val >= _DOWN-ADC_VAR && btn_val <= _DOWN+ADC_VAR)
            {
                cursor += 4;
                click();
                btn_timer = t;
            }
            else if (btn_val >= _A-ADC_VAR && btn_val <= _A+ADC_VAR)
            {
				click();
				btn_timer = t;
                return cursor;
            }
			else if (btn_val >= _B-ADC_VAR && btn_val <= _B+ADC_VAR)
            {
				cursor = 255;
				btn_timer = t;
				return cursor;
			}
        }
        
        if (t - btn_timer > BTN_DELAY)
            btn_timer = 0;
        
        if (cursor > 250)
            cursor = 0;
        else if (cursor >= num_items)
            cursor = num_items-1;
        
        for(byte i=0 ; i<height*4 ; i++)
        {
            col = i % 4;
            row = i / 4;
            if (i<num_items)
                display_block_(&GLYPHS[items[i].glyph*8], top_left.x+col, top_left.y+row, i == cursor);
            else
                display_block(&GLYPHS[0], top_left.x+col, top_left.x+row);
        }
        
    }
}

//http://tech-algorithm.com/articles/nearest-neighbor-image-scaling/
void display_block_embiggened(byte glyph, point top_left)
{
    byte block[8];
    word buffer[16] = {0}; // Initialise to zero
    
    for(byte i=0 ; i<8 ; i++)
        block[i] = pgm_read_byte(&GLYPHS[glyph*8]+i);
    
    //unsigned long x_ratio = 32769;//(unsigned long)((8<<16)/16) +1;
    //unsigned long y_ratio = 32769;//(unsigned long)((8<<16)/16) +1;
    
    byte x2;
    byte y2;
    for (byte i=0;i<16;i++) 
    {
        for (byte j=0;j<16;j++) 
        {
            x2 = (byte)((j*32769)>>16) ;
            y2 = (byte)((i*32769)>>16) ;
            if (block[y2] & (1 << x2))
                buffer[i] |= 1 << j;
        }
    }
    
    set_display_col_row(top_left.x, top_left.y);
    for (byte i=0 ; i<16 ; i++)
    {
        shift_out((buffer[i] >> 8), LSBFIRST);
    }
    
    set_display_col_row(top_left.x, top_left.y+1);
    for (byte i=0 ; i<16 ; i++)
    {
        shift_out((buffer[i] & 0xff), LSBFIRST);
    }
}

void battle_mode(mob_type *player, mob_type *opponent)
{
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
    //display_block(&GLYPHS[32*8], 1, 1);
    //display_block(&GLYPHS[DIGIT_OFFSET*8], 2, 1);
    //display_block(&GLYPHS[DIGIT_OFFSET*8], 3, 1);
    
    display_block(&GLYPHS[102*8], 5, 4);
    display_block(&GLYPHS[103*8], 5+player->num_actions+1, 4);
    
    //TODO: Need to be cleverer about lining these up
    display_block(&GLYPHS[102*8], 5, 2);
    display_block(&GLYPHS[103*8], 5+opponent->num_actions+1, 2);
    
    //draw player TODO: scale x2
    display_block_embiggened(player->glyph, (point){.x=1, .y=6});
    
    //draw opponent TODO: scale x2
    display_block_embiggened(opponent->glyph, (point){.x=13, .y=2});

    byte turn=0;
	byte p_action=0;
    
    for(ever)
    {
        //update countdown
        
        /* Display player health */
        for(byte i=0 ; i<((player->hitpoints & 0xf0) >> 4) ; i++)
        {
            if( i >= (player->hitpoints & 0x0f) )
                display_block(&GLYPHS[104*8], 0+i, 5);
            else
                display_block(&GLYPHS[105*8], 0+i, 5);
        }
        
        /* Display opponent health */
        for(byte i=0 ; i<((opponent->hitpoints & 0xf0) >> 4) ; i++)
        {
            if( i >= (opponent->hitpoints & 0x0f) )
                display_block(&GLYPHS[104*8], 15-i, 1);
            else
                display_block(&GLYPHS[105*8], 15-i, 1);
        }
        
        if(opponent->dead || player->dead)
        {
            delay_ms(1000);
            break;
        }
        
        //choose opponent action(s)
        for(byte i=0 ; i<opponent->num_actions ; i++)
        {
            display_block(&GLYPHS[97*8], 6+i, 2);
        }
        
        // Clear player actions
        for(byte i=0 ; i<player->num_actions ; i++)
            display_block(&GLYPHS[0], 6+i, 4);
        
        byte player_turn = TRUE;
        player->actions = 0;
        //await player actions
        while(player_turn)
        {
            //byte a = display_item_window((point){.x=6, .y=6}, (byte[3]){239, 240, 241}, 3, 3, FALSE);
            byte a = 255;
            while(a == 255)
                a = display_item_window((point){.x=6, .y=6}, &combat_actions[0], 3, 3);
            display_block(&GLYPHS[(98+a)*8], p_action+6, 4);
            player->actions |= (a+1) << (p_action*3);
            p_action += 1;
            
            if (p_action == player->num_actions)
                player_turn = FALSE;
            else
                display_block(&GLYPHS[96*8], 6+p_action, 4);
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
        
        for(byte i=0 ; i<5 ; i++)
        {
            byte pa = (player->actions >> (i*3)) & 0x0007;
            if (pa == 2)
                p_defence_mod += 3;
            else if (pa == 3)
                p_attack_mod += 3;
            else if (pa == 1)
            {
                if ( (rng() % 20) + player->attack + p_attack_mod > opponent->defence + o_defence_mod)
                {
                    if (player->damage >= (opponent->hitpoints & 0x0f))
                    {
                        opponent->hitpoints = opponent->hitpoints & 0xf0;
                        opponent->dead = TRUE;
                        //opponent->glyph = 20;
                        display_block_embiggened(20, (point){.x=13, .y=2});
                        spawn_timer = t;
                    }
                    else
                        opponent->hitpoints -= player->damage;
                }
            }
            
            
            byte oa = (opponent->actions >> (i*3)) & 0x0007;
            if (oa == 2)
            {
                o_defence_mod += 3;
                display_block(&GLYPHS[99*8], 6+i, 2);
            }
            else if (oa == 3)
            {
                o_attack_mod += 3;
                display_block(&GLYPHS[100*8], 6+i, 2);
            }
            
            if (oa == 1)
            {
                display_block(&GLYPHS[98*8], 6+i, 2);
                if ( (rng() % 20) + opponent->attack + o_attack_mod > player->defence + p_defence_mod)
                {
                    if (opponent->damage >= (player->hitpoints & 0x0f))
                    {
                        player->hitpoints = player->hitpoints & 0xf0;
                        player->dead = TRUE;
                        //player->glyph = 20;
                        display_block_embiggened(player->glyph, (point){.x=1, .y=6});
                    }
                    else
                        player->hitpoints -= opponent->damage;
                    
                }
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
    
    byte map_dirty = TRUE;
    
    build_location_portals();
    location *current_location = &village;
    
    /*current_location->mobs[0] = &(mob_type){
        .glyph = (PLAYER_OFFSET+7),   // Blob
        .position = {.x=10, .y=8},
    
        .hitpoints = (4<<4) | 4,
        .damage = 1,
        .attack = 0,
        .defence = 5,
        .num_actions = 1,
        
        .actions = 0b01010101, // blobs just attack
    
        .dead = FALSE,
    };*/
        
    //mob_type mob1 = MOBS[BLOB];
    current_location->mobs[0] = &BLOB; //&mob1; //MOBS[BLOB];
    current_location->mobs[0]->position.x = 10;
    current_location->mobs[0]->position.y = 8;
    
    //mob_type mob2 = MOBS[BLOB];
    current_location->mobs[1] = &BLOB; //&mob2; //MOBS[BLOB];
    current_location->mobs[1]->position.x = 11;
    current_location->mobs[1]->position.y = 5;

    //mob_type mob3 = MOBS[GNOME];
    current_location->mobs[2] = &GNOME;
    current_location->mobs[2]->position.x = 15;
    current_location->mobs[2]->position.y = 1;
        
    current_location->containers[0] = &(container){
        .glyph=54,
        .position={.x=18, .y=6},
        .items={CHICKEN_LEG},
    };
    
    //for(byte i=1 ; i<MAX_CONTAINERS ; i++)
    //{
    //    current_location->containers[i] = NULL_CONTAINER;
   // }

    
    mob_type player = {
        .glyph = PLAYER_OFFSET,
        .position = current_location->portal_in,
    
        .attack = 1,
        .defence = 6,
        .hitpoints = (3<<4) | 3,
        .damage = 2,
        .num_actions = 2,
        
        .actions = 0, // Not applicable to player
    
        .dead = FALSE,
    };
	
	//item inventory[12];
	inventory[0] = APPLE;
	/*inventory[1] = NULL_ITEM;
	inventory[2] = NULL_ITEM;
	inventory[3] = NULL_ITEM;
	inventory[4] = NULL_ITEM;
	inventory[5] = NULL_ITEM;
	inventory[6] = NULL_ITEM;
	inventory[7] = NULL_ITEM;
	inventory[8] = NULL_ITEM;
	inventory[9] = NULL_ITEM;
	inventory[10] = NULL_ITEM;
	inventory[11] = NULL_ITEM;*/
        
    //container loot;
    
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
                    click();
                }
                
                btn_timer = t;
            }
            else if (btn_val >= _RIGHT-ADC_VAR && btn_val <= _RIGHT+ADC_VAR)
            {
                if (!collide_at(current_location, player.position.x+1, player.position.y))
                {
                    player.position.x += 1;
                    map_dirty = TRUE;
                    click();
                }
                btn_timer = t;
            }
            else if (btn_val >= _UP-ADC_VAR && btn_val <= _UP+ADC_VAR)
            {
                if (!collide_at(current_location, player.position.x, player.position.y-1))
                {
                    player.position.y -= 1;
                    map_dirty = TRUE;
                    click();
                }
                
                btn_timer = t;
            }
            else if (btn_val >= _DOWN-ADC_VAR && btn_val <= _DOWN+ADC_VAR)
            {
                if (!collide_at(current_location, player.position.x, player.position.y+1))
                {
                    player.position.y += 1;
                    map_dirty = TRUE;
                    click();
                }
                
                btn_timer = t;
            }
            else if (btn_val >= _A-ADC_VAR && btn_val <= _A+ADC_VAR)
            {
                btn_timer = t;
                if (player.position.x == current_location->portal_out.x && 
                    player.position.y == current_location->portal_out.y &&
                    current_location->return_to != 0)
                {
                    player.position = current_location->portal_in;
                    current_location = current_location->return_to;
                    map_dirty = TRUE;
                    click();
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
                            click();
                            
                            break;
                        }
                    }
                    
                    for (byte i=0 ; i<MAX_CONTAINERS ; i++)
                    {
                        if (player.position.x == current_location->containers[i]->position.x && 
                            player.position.y == current_location->containers[i]->position.y)
                        {
                            // Display contents
                            crap_beep(_A4, 150);
                            delay_ms(50);
                            crap_beep(_A5, 100);
                            display_window((point){5, 2}, 6, 4);
                            byte selected = display_item_window(
                                            (point){.x=6, .y=3},
                                            &current_location->containers[i]->items[0], 
                                            CONTAINER_SIZE,
                                            4
                                           );
                            
							if (selected < 255)
							{
								for(byte slot=0 ; slot<PLAYER_INVENTORY ; slot++)
								{
									if (inventory[slot].glyph == 0 && current_location->containers[i]->items[selected].glyph != 0)
									{
										inventory[slot] = current_location->containers[i]->items[selected];
										current_location->containers[i]->items[selected] = NULL_ITEM;
									}
								}
							}
                            
                            map_dirty = TRUE;
                            
                            break;
                        }
                    }
                }
            }
            else if (btn_val >= _B-ADC_VAR && btn_val <= _B+ADC_VAR)
            {
                map_dirty = TRUE;
                
                click();
                
                btn_timer = t;
            }
            else if (btn_val >= _C-ADC_VAR && btn_val <= _C+ADC_VAR)
            {
                map_dirty = TRUE;
                    
                click();
                
                display_window((point){5, 1}, 6, 5);
                byte selected = 0;
                cursor = 0;
                while (selected < 255)
                {
                    for(byte i=0 ; i<((player.hitpoints & 0xf0) >> 4) ; i++)
                    {
                        if( i >= (player.hitpoints & 0x0f) )
                            display_block(&GLYPHS[104*8], 5+i, 0);
                        else
                            display_block(&GLYPHS[105*8], 5+i, 0);
                    }
                    
                    selected = display_item_window((point){.x=6, .y=2}, &inventory[0], PLAYER_INVENTORY, 4);
                    if (selected < 255)
                    {   
                        if (inventory[selected].attributes & CONSUMABLE)
                        {
                            //TODO: if I ever implement mana, need to check here
                            if ((player.hitpoints & 0x0f) + (inventory[selected].attributes & HP) > player.hitpoints >> 4)
                                player.hitpoints = (player.hitpoints & 0xf0) | (player.hitpoints >> 4);
                            else
                                player.hitpoints += (inventory[selected].attributes & HP);
                            
                            inventory[selected] = NULL_ITEM;
                        }
                        click();
                    }
                }
                
                btn_timer = t;
            }
            /*else if (btn_val >= _D)
            {
                map_dirty = TRUE;
                    
                click();
                
                display_window((point){0, 0}, 16, 8);
                display_string(PSTR("PAUSED"), 5, 0);
                
                //TODO: Display pause screen
                //Player Health / Mana
                //Player Level / XP (when implemented)
                //Player kills? maybe
                
                while(analog_read(ADC2) > 50){}
                //btn_timer = t;
            }*/
        }
        
        if (t - btn_timer >= BTN_DELAY)
            btn_timer = 0;
        if (t - spawn_timer >= SPAWN_DELAY)
            spawn_timer = 0;
        
        if (player.position.x == 255)
            player.position.x = 0;
        if (player.position.x > current_location->width-1)
            player.position.x = current_location->width-1;
        
        if (player.position.y == 255)
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
        
        //display_player(&player);
        display_block(&GLYPHS[player.glyph*8], player.position.x-viewport_col, (player.position.y-viewport_row));
        if (player.dead)
        {
            display_window((point){5, 2}, 6, 4);
            display_block_embiggened(20, (point){.x=7, .y=3});
            
            while(player.dead) 
            {
                btn_val = analog_read(ADC2);
                if (btn_val >= _AC-ADC_VAR && btn_val <= _AC+ADC_VAR )
                {
                    player.dead = FALSE;
                    player.hitpoints |= 1;
                    inventory[0] = APPLE;
                    for (byte i=1 ; i<PLAYER_INVENTORY ; i++)
                        inventory[i] = NULL_ITEM;
                    
                    player.position.x = 7;
                    player.position.y = 5;
                    map_dirty = TRUE;
                }
            }
        }
        
        mob_type *opponent = 0; //update_mobs(current_location, &player);
        for(byte i=0 ; i<MAX_MOBS ; i++)
        {
            if (current_location->mobs[i] != 0 && !current_location->mobs[i]->dead)
            {
                //TODO: update the mob
                
                if (current_location->mobs[i]->position.x >= viewport_col && current_location->mobs[i]->position.x < viewport_col+SCREEN_COLUMNS && 
                    current_location->mobs[i]->position.y >= viewport_row && current_location->mobs[i]->position.y < viewport_row+SCREEN_ROWS )
                {
                    display_block(&GLYPHS[current_location->mobs[i]->glyph*8], current_location->mobs[i]->position.x-viewport_col, (current_location->mobs[i]->position.y-viewport_row));
                }
                if (current_location->mobs[i]->position.x == player.position.x && 
                    current_location->mobs[i]->position.y == player.position.y &&
                    !current_location->mobs[i]->dead)
                {
                    opponent = current_location->mobs[i];
                    break; // TODO: This stops other mobs getting updated
                }
            }
            else if (current_location->mobs[i]->dead)
            {
                if (spawn_timer == 0)
                {
                    current_location->mobs[i]->dead = FALSE;
                    current_location->mobs[i]->hitpoints = current_location->mobs[i]->hitpoints | (current_location->mobs[i]->hitpoints >> 4);
                    current_location->mobs[i]->position.x = (rng() % 5) + 13;
                    current_location->mobs[i]->position.y = rng() % 3;
                    spawn_timer = t;
                }
            }
        }
        
        for(byte i=0 ; i<MAX_CONTAINERS ; i++)
        {
            if (current_location->containers[i] != 0 && 
                current_location->containers[i]->position.x >= viewport_col && 
                current_location->containers[i]->position.x < viewport_col+SCREEN_COLUMNS && 
                current_location->containers[i]->position.y >= viewport_row && 
                current_location->containers[i]->position.y < viewport_row+SCREEN_ROWS )
            {
                display_block(&GLYPHS[current_location->containers[i]->glyph*8], current_location->containers[i]->position.x-viewport_col, (current_location->containers[i]->position.y-viewport_row));
                
                if (current_location->containers[i]->glyph == 20)
                {
                    byte empty = TRUE;
                    for (byte item=0 ; item<CONTAINER_SIZE ; item++)
                    {
                        if (current_location->containers[i]->items[item].glyph != 0)
                            empty = FALSE;
                    }
                    if (empty)
                        current_location->containers[i] = 0;
                }
            }
        }
        
        if (opponent != 0)// && !opponent->dead)
        {
            crap_beep(_A4, 20);
            delay_ms(10);
            crap_beep(_A5, 35);
            battle_mode(&player, opponent);
            
            if (!player.dead)
            {
                for(byte i=0 ; i<MAX_CONTAINERS ; i++)
                {
                    if (current_location->containers[i] == 0)
                    {
                        crap_beep(_A4, 500);
                        
                        byte loot = rng() % 2;
                        if (loot == 0)
                        {
                            current_location->containers[i] = &(container){
                                .glyph=20,
                                .position={.x=opponent->position.x, .y=opponent->position.y},
                                .items={APPLE, },
                            };
                        }
                        else if (loot == 1)
                        {
                            current_location->containers[i] = &(container){
                                .glyph=20,
                                .position={.x=opponent->position.x, .y=opponent->position.y},
                                .items={CHICKEN_LEG, },
                            };
                        }
    
                        break;
                    }
                }
            }
            
            map_dirty = TRUE;
        }
        
        //display_mobs(current_location);
        /*for(byte i=0 ; i<MAX_MOBS ; i++)
        {
            if (current_location->mobs[i])
            {
                if (current_location->mobs[i]->position.x >= viewport_col && current_location->mobs[i]->position.x < viewport_col+SCREEN_COLUMNS && 
                    current_location->mobs[i]->position.y >= viewport_row && current_location->mobs[i]->position.y < viewport_row+SCREEN_ROWS )
                {
                    display_block(&GLYPHS[current_location->mobs[i]->glyph*8], current_location->mobs[i]->position.x-viewport_col, (current_location->mobs[i]->position.y-viewport_row));
                }
            }
        }*/
        
        //delta = millis() - t;
        //display_hud(delta);
    }
}