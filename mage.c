#include <avr/io.h>

#include "mage.h"
#include "glyphs.h"
#include "maps.h"
#include "beep.h"

byte HUD[4] = {0x00, 0x00, 0x00, 0x00};
unsigned int btn_timers[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        
int x = 3;
int y = 3;

int viewport_col = 0;
int viewport_row = 0;


void send_command(byte command)
{
    for (byte i = 0; i < 8; i++)  
    {
        if ( command & (1 << (7 - i)) )
        {
            PORTB |= 1 << SDA;
        }
        else
        {
            PORTB &= ~(1 << SDA);
        }
        
        PORTB |= 1 << SCL;      // HIGH
        PORTB &= ~(1 << SCL);   // LOW
    }
}

void shift_out(byte val, byte order)
{
    byte b;
    for (byte i = 0; i < 8; i++)  
    {
        if (order == LSBFIRST)
        {
            b = val & (1 << i);
        }
        else
        {
            b = val & (1 << (7 - i));
        }
        
        if ( b )
        {
            PORTB |= 1 << SDA;
        }
        else
        {
            PORTB &= ~(1 << SDA);
        }
        
        PORTB |= 1 << SCL; // HIGH
        PORTB &= ~(1 << SCL); // LOW
    }
}

/* Un-rolling the loop makes this much faster */
void shift_out_block(const byte *block)
{
    byte b;
    for (byte i = 0; i < 8; i++)  
    {
        b = pgm_read_byte(block+i);
        
        if ( b & (1 << 0) )
        {
            PORTB |= 1 << SDA;
        }
        else
        {
            PORTB &= ~(1 << SDA);
        }
        
        PORTB |= 1 << SCL; // HIGH
        PORTB &= ~(1 << SCL); // LOW
        
        if ( b & (1 << 1) )
        {
            PORTB |= 1 << SDA;
        }
        else
        {
            PORTB &= ~(1 << SDA);
        }
        
        PORTB |= 1 << SCL; // HIGH
        PORTB &= ~(1 << SCL); // LOW
        
        if ( b & (1 << 2) )
        {
            PORTB |= 1 << SDA;
        }
        else
        {
            PORTB &= ~(1 << SDA);
        }
        
        PORTB |= 1 << SCL; // HIGH
        PORTB &= ~(1 << SCL); // LOW
        
        if ( b & (1 << 3) )
        {
            PORTB |= 1 << SDA;
        }
        else
        {
            PORTB &= ~(1 << SDA);
        }
        
        PORTB |= 1 << SCL; // HIGH
        PORTB &= ~(1 << SCL); // LOW
        
        if ( b & (1 << 4) )
        {
            PORTB |= 1 << SDA;
        }
        else
        {
            PORTB &= ~(1 << SDA);
        }
        
        PORTB |= 1 << SCL; // HIGH
        PORTB &= ~(1 << SCL); // LOW
        
        if ( b & (1 << 5) )
        {
            PORTB |= 1 << SDA;
        }
        else
        {
            PORTB &= ~(1 << SDA);
        }
        
        PORTB |= 1 << SCL; // HIGH
        PORTB &= ~(1 << SCL); // LOW
        
        if ( b & (1 << 6) )
        {
            PORTB |= 1 << SDA;
        }
        else
        {
            PORTB &= ~(1 << SDA);
        }
        
        PORTB |= 1 << SCL; // HIGH
        PORTB &= ~(1 << SCL); // LOW
        
        if ( b & (1 << 7) )
        {
            PORTB |= 1 << SDA;
        }
        else
        {
            PORTB &= ~(1 << SDA);
        }
        
        PORTB |= 1 << SCL; // HIGH
        PORTB &= ~(1 << SCL); // LOW
    }
}


void initialise_oled(void)
{
    PORTB &= ~(1 << DC);        // LOW (Command Mode)
    send_command(0xAE);         // DISPLAYOFF
    
    send_command(0xD5);         // SETDISPLAYCLOCKDIV
    send_command(0x80);         // the suggested ratio 0x80

    send_command(0xA8 );        // SSD1306_SETMULTIPLEX
    send_command(HEIGHT - 1);

    send_command(0xD3 );        // SETDISPLAYOFFSET
    send_command(0x0);          // no offset
    send_command(0x40  | 0x0);  // SETSTARTLINE line #0
  
    send_command(0x8D);        // CHARGEPUMP
    send_command(0x14);         // Not External Vcc
  
    send_command(0x20 );        // MEMORYMODE
    send_command(0x00);         // 0x0 act like ks0108
    send_command(0xA0  | 0x1);  // SEGREMAP
    send_command(0xC8 );        // COMSCANDEC


    send_command(0xDA);        // SETCOMPINS
    send_command(0x12);
  
    send_command(0x81 );        // SETCONTRAST
    send_command(0xCF);         // Not External Vcc


    send_command(0xD9 );        // SETPRECHARGE
    send_command(0xF1);         // Not External Vcc
  
    send_command(0xDB);        // SETVCOMDETECT
    send_command(0x40);
  
    send_command(0xA4 );        // DISPLAYALLON_RESUME
    send_command(0xA6 );        // NORMALDISPLAY

    send_command(0x2E );        // DEACTIVATE_SCROLL

    send_command(0xAF);         // DISPLAYON
}

void clear_display(void)
{
    PORTB &= ~(1 << DC);        // LOW
    send_command(0x21);         //  COLUMNADDR
    send_command(0);            // Column start address (0 = reset)
    send_command(WIDTH - 1);    // Column end address (127 = reset)

    send_command(0x22);         //  PAGEADDR
    send_command(0);            // Page start address (0 = reset)
    send_command(7);            // Page end address // 64 lines
    
    PORTB |= 1 << DC;           // HIGH
    
    for (byte i=0 ; i<128 ; i++)
    {
        shift_out_block(&GLYPHS[0]);
    }
}

void display_off(void)
{
    PORTB &= ~(1 << DC);    // LOW
    send_command(0xAE);     // DISPLAYOFF
}

void display_on(void)
{
    PORTB &= ~(1 << DC);    // LOW
    send_command(0xAF);     // DISPLAYON
}

void display_image(const byte *img, unsigned int col, unsigned int row, unsigned int width, unsigned int height)
{
    for (int h=0 ; h<height ; h++)
    {
        PORTB &= ~(1 << DC);        // LOW
        
        send_command(0x21);         //  COLUMNADDR
        send_command(col*8);            // Column start address (0 = reset)
        send_command(WIDTH - 1);    // Column end address (127 = reset)
        
        send_command(0x22);         //  PAGEADDR
        send_command(row+h);        // Page start address (0 = reset)
        send_command(7);            // Page end address // 64 lines
        
        PORTB |= 1 << DC;           // HIGH
    
        for (int w=0 ; w<width ; w++)
            shift_out_block(&img[(width * h + w)*8]);
    }
    
    PORTB &= ~(1 << DC);        // LOW
    send_command(0x21);         //  COLUMNADDR
    send_command(0);            // Column start address (0 = reset)
    send_command(WIDTH - 1);    // Column end address (127 = reset)

    send_command(0x22);         //  PAGEADDR
    send_command(0);            // Page start address (0 = reset)
    send_command(7);            // Page end address // 64 lines
    
    PORTB |= 1 << DC;           // HIGH
}

void display_map(const byte *m, int map_cols, int map_rows)
{
    viewport_col = x-COLUMNS/2;
    viewport_row = y-ROWS/2;
    
    if (viewport_col < 0)
        viewport_col = 0;
    if (viewport_row < 0)
        viewport_row = 0;
    
    if (viewport_col + COLUMNS > map_cols)
        viewport_col = map_cols - COLUMNS;
    if (viewport_row + ROWS > map_rows)
        viewport_row = map_rows - ROWS;
    
    int col, row;
    
    for (row=0 ; row<ROWS ; row++)
    {
        for (col=0 ; col<COLUMNS ; col++)
        {
            int i = map_cols * (viewport_row+row) + (viewport_col+col);
            if (i > map_cols * map_rows)
                shift_out_block(&GLYPHS[0]);
            else
                shift_out_block(&GLYPHS[pgm_read_byte(&m[ i ])*8]);
        }
    }
    
    // Drawing sections of screen
    // https://www.ccsinfo.com/forum/viewtopic.php?p=217165
}

void display_player(void)
{
    PORTB &= ~(1 << DC);        // LOW
    send_command(0x21);         //  COLUMNADDR
    send_command((x-viewport_col)*8);          // Column start address (0 = reset)
    send_command(WIDTH - 1);    // Column end address (127 = reset)

    send_command(0x22);         //  PAGEADDR
    send_command((y-viewport_row));            // Page start address (0 = reset)
    send_command(7);            // Page end address // 64 lines
    
    PORTB |= 1 << DC;           // HIGH
    
    shift_out_block(&GLYPHS[PLAYER_OFFSET*8]);
    
    PORTB &= ~(1 << DC);        // LOW
    send_command(0x21);         //  COLUMNADDR
    send_command(0);            // Column start address (0 = reset)
    send_command(WIDTH - 1);    // Column end address (127 = reset)

    send_command(0x22);         //  PAGEADDR
    send_command(0);            // Page start address (0 = reset)
    send_command(7);            // Page end address // 64 lines
    
    PORTB |= 1 << DC;           // HIGH
}

/*void display_hud(void)
{
    PORTB &= ~(1 << DC);        // LOW
    send_command(0x21);         //  COLUMNADDR
    send_command(10*8);            // Column start address (0 = reset)
    send_command(WIDTH - 1);    // Column end address (127 = reset)

    send_command(0x22);         //  PAGEADDR
    send_command(0);            // Page start address (0 = reset)
    send_command(7);            // Page end address // 64 lines
    
    PORTB |= 1 << DC;           // HIGH
    
    shift_out_block(&GLYPHS[(HUD[0])*8]);
    shift_out_block(&GLYPHS[(HUD[1]+DIGIT_OFFSET)*8]);
    shift_out_block(&GLYPHS[(HUD[2]+DIGIT_OFFSET)*8]);
    shift_out_block(&GLYPHS[(HUD[3]+DIGIT_OFFSET)*8]);
    
    PORTB &= ~(1 << DC);        // LOW
    send_command(0x21);         //  COLUMNADDR
    send_command(0);            // Column start address (0 = reset)
    send_command(WIDTH - 1);    // Column end address (127 = reset)

    send_command(0x22);         //  PAGEADDR
    send_command(0);            // Page start address (0 = reset)
    send_command(7);            // Page end address // 64 lines
    
    PORTB |= 1 << DC;           // HIGH
}*/

byte collide_at(int col, int row)
{
    // odd collide, even passable, TODO: above ~128
    return pgm_read_byte(&MAP1[ MAP_1_COLS * row + col ]) % 2;
}

int main (void) 
{
    // Setup
    DDRB = (1<<SDA) | (1<<DC) | (1<<SCL) | (1<<SND); // Configure Outputs
    
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
    send_command(0xA7 );        // INVERTEDDISPLAY
    PORTB |= 1 << DC;           // HIGH
    
    unsigned int t;
    //unsigned int delta = 0;
    unsigned int btn_val = 0;
    //unsigned int hud_timer = 0;
    
    byte map_dirty = TRUE;
    
    for(ever)
    {
        t = millis();
        
        if (map_dirty)
        {
            display_map(&MAP1[0], MAP_1_COLS, MAP_1_ROWS);
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
            if (!collide_at(x-1, y))
            {
                x -= 1;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            
            btn_timers[0] = t;
        }
        else if (btn_val >= _RIGHT-ADC_VAR && btn_val <= _RIGHT+ADC_VAR && btn_timers[1] == 0)
        {
            if (!collide_at(x+1, y))
            {
                x += 1;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            btn_timers[1] = t;
        }
        else if (btn_val >= _UP-ADC_VAR && btn_val <= _UP+ADC_VAR && btn_timers[2] == 0)
        {
            if (!collide_at(x, y-1))
            {
                y -= 1;
                map_dirty = TRUE;
                crap_beep(SND, _A9, 5);
            }
            
            btn_timers[2] = t;
        }
        else if (btn_val >= _DOWN-ADC_VAR && btn_val <= _DOWN+ADC_VAR && btn_timers[3] == 0)
        {
            if (!collide_at(x, y+1))
            {
                y += 1;
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
        
        if (x < 0)
            x = 0;
        if (x > MAP_1_COLS-1)
            x = MAP_1_COLS-1;
        
        if (y < 0)
            y = 0;
        if (y > MAP_1_ROWS-1)
            y = MAP_1_ROWS-1;
        
        //display_hud();
        display_player();
        
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