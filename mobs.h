#ifndef MOBS_H
# define MOBS_H

#include "utils.h"

#define MAX_MOBS 8

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
    // aggro_radius, abilities?, hit, block&dodge chance
} mob_type;

#define BLOB  0
#define GNOME 1

mob_type MOBS[] = {
    (mob_type) {
        .glyph = 67,   // Blob
        .position = {.x=0, .y=0},
    
        .hitpoints = (4<<4) | 4,
        .damage = 1,
        .attack = 0,
        .defence = 5,
        .num_actions = 1,
        
        .tactics = 0b01010101, // blobs just attack
    
        .dead = FALSE,
    },
    
    (mob_type) {
        .glyph = 63,   // Gnome
        .position = {.x=0, .y=0},
    
        .hitpoints = (6<<4) | 6,
        .damage = 2,
        .attack = 1,
        .defence = 7,
        .num_actions = 2,
        
        .tactics = 0b10011001, // Gnomes defend/attack
    
        .dead = FALSE,
    },
};
    

#endif