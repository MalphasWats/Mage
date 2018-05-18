#ifndef MOBS_H
# define MOBS_H

#include "utils.h"

typedef struct mob_type {
    int glyph;
    point position;
    
    int hitpoints;
    
    int attack_damage;
    byte num_attacks;
    
    byte dead;
    // aggro_radius, abilities?, hit, block&dodge chance
} mob_type; 

#endif