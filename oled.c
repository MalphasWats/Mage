#include <avr/io.h>

#include "oled.h"

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
void shift_out_block(const byte *block, byte inverted)
{
    byte b;
    for (byte i = 0; i < 8; i++)  
    {
        // see this: https://gcc.gnu.org/onlinedocs/gcc-4.7.0/gcc/Named-Address-Spaces.html
        b = pgm_read_byte(block+i);
        if (inverted)
            b = ~b;
        
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
    DDRB |= (1<<SDA) | (1<<DC) | (1<<SCL);
    
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

    //send_command(0x2E );        // DEACTIVATE_SCROLL

    send_command(0xAF);         // DISPLAYON
}

void clear_display(void)
{
    for (byte row=0 ; row<SCREEN_ROWS ; row++)
    {
        set_display_col_row(0, row);
        
        for (byte col=0 ; col<SCREEN_COLUMNS ; col++)
        {
            shift_out_block(&BLANK[0], FALSE);
        }
    }
}

void display_off(void)
{
    PORTB &= ~(1 << DC);    // LOW
    send_command(0xAE);     // DISPLAYOFF
    PORTB |= 1 << DC;           // HIGH
}

void display_on(void)
{
    PORTB &= ~(1 << DC);    // LOW
    send_command(0xAF);     // DISPLAYON
    PORTB |= 1 << DC;           // HIGH
}

void display_image(const byte *img, byte col, byte row, byte width, byte height)
{
    for (byte h=0 ; h<height ; h++)
    {
        set_display_col_row(col, row+h);
        
        for (byte w=0 ; w<width ; w++)
            shift_out_block(&img[(width * h + w)*8], FALSE);
    }
}

void set_display_row(byte row)
{
    set_display_col_row(0, row);
}

void set_display_col_row(byte col, byte row)
{
    PORTB &= ~(1 << DC);        // LOW
    
    send_command(0xB0 + row);         //  PAGEADDR
    send_command((col*8 + SCREEN_RAM_OFFSET) & 0x0F);            // Column start address (0 = reset)
    send_command(0x10 | ((col*8 + SCREEN_RAM_OFFSET) >> 4));         //  LOW COL ADDR
    
    PORTB |= 1 << DC;           // HIGH
}

void display_block(const byte *block, byte col, byte row)
{
    set_display_col_row(col, row);
    shift_out_block(block, FALSE);
}

void display_block_(const byte *block, byte col, byte row, byte inverted)
{
    set_display_col_row(col, row);
    shift_out_block(block, inverted);
}