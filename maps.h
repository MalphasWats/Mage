#ifndef MAPS_H
# define MAPS_H

#include "utils.h"

#define MAX_PORTALS 8
    
typedef struct point {
    int x;
    int y;
} point;


typedef struct location {
    const point portal_in;
    const point portal_out;
    
    const byte *map;
    const byte width;
    const byte height;
    
    struct location *portals[MAX_PORTALS];
    
    point player;
    
    struct location *return_to;
} location; 
    
static const byte VILLAGE[] PROGMEM = {

	139, 139, 139, 139, 139, 139, 139, 139, 143, 143, 143, 143, 143, 143, 143, 143, 
	143, 143, 143, 143, 139, 139, 139,   0,   0,   0,   0,   0, 132,   0,   0,   0, 

	145, 143, 143, 143, 143, 143, 143, 143, 133, 134,   0,   0,   0, 132,   0, 128, 
	  0,   0, 175,   0, 143, 143, 143, 139, 139,   0,   0,   0,   0, 167, 169,   0, 

	145,   0,   0,   0,   0,   0, 132,   0,   0,   0,   0,   0,   0, 151,   0, 153, 
	  0,   0, 179, 181,   0,   0,   0, 143, 145,   0, 132,   0,   0, 171, 174,   0, 

	145,   0, 128,   0, 159, 161,   0,   0,   0, 128,   0,   0,   0, 147, 148, 149, 
	  0,   0,   0,   0, 159, 161,   0,   0, 145,   0,   0,   0,   0,   0, 138,   0, 

	145,   0, 132,   0, 163, 166,   0,   0,   0,   0,   0, 159, 161,   0,   0,   0, 
	  0,   0,   0, 181, 163, 166, 131,   0, 145, 128,   0, 129,   0, 134,   0,   0, 

	145,   0,   0, 129,   0, 138,   0,   0, 134,   0,   0, 163, 166,   0,   0, 132, 
	  0,   0,   0,   0,   0, 138,   0,   0, 145,   0,   0,   0,   0,   0,   0, 130, 

	145,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 138,   0,   0,   0, 
	  0, 132,   0,   0, 130,   0,   0,   0, 145,   0, 139,   0,   0,   0, 139,   0, 

	145,   0,   0, 159, 161,   0,   0,   0, 144,   0, 140,   0,   0,   0, 142,   0, 
	  0,   0,   0, 140,   0,   0,   0,   0, 145, 139, 141, 134, 144,   0, 141, 139, 

	145,   0,   0, 163, 166,   0,   0,   0,   0,   0,   0,   0,   0,   0, 134,   0, 
	144,   0,   0,   0,   0, 144,   0,   0, 143, 143, 143,   0, 138, 134, 143, 145, 

	145, 131,   0,   0, 138,   0,   0,   0, 134,   0, 159, 161,   0, 135,   0,   0, 
	  0,   0, 152,   0,   0,   0,   0, 142,   0,   0, 140,   0,   0,   0, 150, 145, 

	145,   0, 133,   0,   0,   0,   0, 138,   0, 135, 163, 166, 137,   0,   0,   0, 
	  0,   0,   0,   0, 129,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 145, 

	145, 146, 146, 131,   0,   0,   0,   0,   0,   0,   0, 138,   0,   0,   0,   0, 
	138,   0,   0, 142,   0,   0,   0,   0, 136, 136, 136, 136, 136, 136, 136, 145, 

	145, 139, 139, 139, 139,   0,   0, 142,   0,   0,   0,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,   0,   0,   0,   0, 136, 136, 136, 154, 136, 136, 136, 145, 

	143, 143, 143, 143, 145,   0,   0,   0,   0,   0, 134,   0,   0, 139, 134,   0, 
	134, 139,   0,   0, 132, 133, 128,   0, 136, 136, 136, 136, 136, 136, 136, 145, 

	130, 155, 157, 128, 145, 139, 139, 139, 139, 139, 139, 139, 139, 141,   0, 144, 
	  0, 141, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 145, 

	  0, 128,   0, 132, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143,   0, 138, 
	  0, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 143, 

};

location village = { 
    .portal_in={15, 14}, 
    .portal_out={0, 0}, 
    .map=&VILLAGE[0],
    .width=32,
    .height=16,
    .player={15, 14}, 
    .return_to=0 
};


static const byte HOUSE[] PROGMEM = {
	  0,   0,   0,   0,  175, 181, 181, 181, 181, 181, 181, 175,   0,   0,   0,   0,
	  0,   0,   0,   0,  177, 158,   0, 165,   0, 162,   0, 177,   0,   0,   0,   0,  
	  0,   0,   0,   0,  177,   0,   0,   0,   0,   0, 150, 177,   0,   0,   0,   0,  
	  0,   0,   0,   0,  177,   0,   0,   0, 182,   0,   0, 177,   0,   0,   0,   0,  
	  0,   0,   0,   0,  177, 173,   0,   0, 183, 184,   0, 177,   0,   0,   0,   0,  
	  0,   0,   0,   0,  177, 189,   0,   0,   0,   0,   0, 177,   0,   0,   0,   0,  
	  0,   0,   0,   0,  177,   0,   0,   0,   0,   0,   0, 177,   0,   0,   0,   0,  
	  0,   0,   0,   0,  179, 181, 181, 181,   0, 181, 181, 179,   0,   0,   0,   0,  
};

location house = { 
    .portal_in={.x=11, .y=10}, 
    .portal_out={.x=8, .y=7}, 
    .map=&HOUSE[0],
    .width=16,
    .height=8,
    .player={0, 0}, 
    .return_to=&village
};


void build_location_portals()
{
    village.portals[0] = &house;
}

#endif