#ifndef MOBS_H
# define MOBS_H

#include "utils.h"

#define MAX_MOBS 3

#define CONSUMABLE  1<<3
#define HP          0x0007

#define NULL_ITEM       (item){.glyph=0, .attributes=0}
#define APPLE           (item){.glyph=26, .attributes=0b0000000000001001}
#define CHICKEN_LEG     (item){.glyph=25, .attributes=0b0000000000001011}

// SLOT     ATK     DEF     DAM  CONSUMABLE     HP
//  000     000     000     000      0          000

typedef struct item {
    byte glyph;
                        // SLOT     ATK     DEF     DAM  CONSUMABLE     HP
    word attributes;    //  000     000     000     000      0          000
} item;

typedef struct mob_type {
    byte glyph;
    point position;
    
    byte hitpoints;
    
    byte attack;
    byte defence;
    byte damage;
    
    byte num_actions;
    
    word actions; // 0 000 000 000 000 000 - 000 = no action, 001 = attack, 010 = block, 011 = focus
    
    byte dead;
} mob_type;

//#define BLOB   0
//#define GNOME  1

#define BLOB (mob_type){.glyph = 18, .hitpoints = (2<<4) | 2, .damage = 1, .attack = 0, .defence = 5, .num_actions = 1, .actions = 0b0000000000000001, .dead = FALSE, }

#define GNOME (mob_type){.glyph = 14, .hitpoints = (3<<4) | 3, .damage = 1, .attack = 2, .defence = 4, .num_actions = 2, .actions = 0b0000000000001010, .dead = FALSE, }

/*mob_type MOBS[] = {
    (mob_type){
        .glyph = 18,   // Blob
        .position = {.x=0, .y=0},
    
        .hitpoints = (2<<4) | 2,
        .damage = 1,
        .attack = 0,
        .defence = 5,
        .num_actions = 1,
        
        .actions = 0b0000000000000001, // blobs just attack
    
        .dead = FALSE,
    },
            
    (mob_type){
        .glyph = 14,   // Gnome
        .position = {.x=0, .y=0},
    
        .hitpoints = (3<<4) | 3,
        .damage = 1,
        .attack = 2,
        .defence = 4,
        .num_actions = 2,
        
        .actions = 0b0000000000001010, // Gnome defends, attacks
    
        .dead = FALSE,
    },
};*/

#endif