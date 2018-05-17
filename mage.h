#ifndef MAGE_H
# define MAGE_H

#define F_CPU 8000000

#include <avr/pgmspace.h>

#include "utils.h"
#include "maps.h"

#define SPLASH_DELAY    1500
#define BTN_DELAY       180

/* Button ADC Values */
#define _UP     688
#define _DOWN   852
#define _LEFT   930
#define _RIGHT  678
#define _A      514
#define _B      312
#define _C      254
#define _D      1021

#define ADC_VAR 2

        
/*  0000  0      1000  8
    0001  1      1001  9
    0010  2      1010  A
    0011  3      1011  B
    0100  4      1100  C
    0101  5      1101  D
    0110  6      1110  E
    0111  7      1111  F */
        
static const byte LOGO[] PROGMEM = {
    
    0x00, 0x00, 0x04, 0x08, 0x18, 0x37, 0x3F, 0x0F,     0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,     // HAT (Top) 
    0x00, 0x00, 0x3F, 0x7E, 0x10, 0x38, 0x1C, 0x0E,     0x04, 0x02, 0x0E, 0x1C, 0x38, 0x10, 0x3F, 0x7E,     // M (Top)
    0x00, 0x00, 0x3F, 0x7E, 0x81, 0xC1, 0xC1, 0xC1,     0xC0, 0xC1, 0xC1, 0xC1, 0xC1, 0xC0, 0xBF, 0x7E,     // A
    0x00, 0x00, 0x3F, 0x7E, 0x80, 0xC0, 0xC0, 0xC0,     0xC0, 0xC1, 0xC1, 0xC1, 0xC1, 0xC0, 0x80, 0x00,     // G
    0x00, 0x00, 0x3F, 0x7E, 0x81, 0xC1, 0xC1, 0xC1,     0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x00,     // E    
        
    0x00, 0x38, 0x5C, 0x2C, 0xEC, 0xEC, 0xEC, 0xEC,     0xEC, 0xEC, 0x2C, 0x5C, 0x3C, 0x38, 0x00, 0x00,     // HAT (Bottom)  
    0x00, 0x00, 0x7C, 0xFE, 0x00, 0x00, 0x00, 0x00,     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0xFC,     // M
    0x00, 0x00, 0x7C, 0xFE, 0x80, 0x80, 0x80, 0x80,     0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x7E, 0xFC,     // A
    0x00, 0x00, 0x7C, 0xFE, 0x01, 0x03, 0x03, 0x03,     0x03, 0x03, 0x83, 0x83, 0x83, 0x81, 0x7E, 0xFC,     // G
    0x00, 0x00, 0x7C, 0xFE, 0x01, 0x83, 0x83, 0x83,     0x83, 0x03, 0x03, 0x03, 0x03, 0x02, 0x00, 0x00,     // E
};

void display_map(location *loc);
void display_player(location *loc);

byte collide_at(location *loc, int row, int col);

#endif