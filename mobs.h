#ifndef MOBS_H
# define MOBS_H

#include "utils.h"

#define MAX_MOBS 8

typedef struct mob_type {
    byte glyph;
    point position;
    
    byte hitpoints;
    
    byte attack_damage;
    byte num_attacks;
    
    byte tactics; // 00 00 00 00 - 00 = wait, 01 = block, 10 = attack, 11 = reserved
    
    byte dead;
    // aggro_radius, abilities?, hit, block&dodge chance
} mob_type; 

#endif