#ifndef MOBS_H
# define MOBS_H

#include "utils.h"

#define MAX_MOBS 8

typedef struct mob_type {
    byte glyph;
    point position;
    
    byte hitpoints;
    
    byte attack;
    byte defence;
    byte damage;
    
    byte num_actions;
    
    byte tactics; // 00 00 00 00 - 00 = no action, 01 = block, 10 = attack, 11 = focus
    
    byte dead;
    // aggro_radius, abilities?, hit, block&dodge chance
} mob_type; 

#endif