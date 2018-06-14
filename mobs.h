#ifndef MOBS_H
# define MOBS_H

#include "utils.h"

#define MAX_MOBS 8

#define NULL_ITEM (item){.glyph=0, .attributes=0}
#define APPLE (item){.glyph=103, .attributes=0b0000001000100010}

// RESERVED EQUIPPABLE EQUIPPED CONSUMABLE ATK DEF DAM HP MAN  VALUE
//  0000      0          0        1         0   0   0   1  0   0010

typedef struct item {
    byte glyph;
                        // RESERVED EQUIPPABLE EQUIPPED CONSUMABLE ATK DEF DAM HP MAN  VALUE
    word attributes;    //  0000      0          0        0         0   0   0   0  0   0000
} item;

typedef struct mob_type {
    byte glyph;
    point position;
    
    byte hitpoints;
    
    byte attack;
    byte defence;
    byte damage;
    
    byte num_actions;
    
    byte tactics; // 00 00 00 00 - 00 = no action, 01 = attack, 10 = block, 11 = focus
    
    byte dead;
} mob_type;

#endif