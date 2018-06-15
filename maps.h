#ifndef MAPS_H
# define MAPS_H

#include "utils.h"
#include "mobs.h"

#define MAX_PORTALS 8
#define MAX_CONTAINERS 1
#define CONTAINER_SIZE 8

typedef struct container {
    const point position;
    item items[CONTAINER_SIZE];
} container;

typedef struct location {
    const point portal_in;
    const point portal_out;
    
    const byte *map;
    const byte width;
    const byte height;
    
    struct location *portals[MAX_PORTALS];
    mob_type *mobs[MAX_MOBS];
    
    container *containers[MAX_CONTAINERS];
    
    struct location *return_to;
} location; 


    
static const byte VILLAGE[] PROGMEM = {


	139, 139, 139, 139, 139, 139, 143, 143, 143, 139, 
	139, 133, 130,   0,   0,   0, 128,   0, 167, 169,

	145, 143, 143, 143, 143, 143, 133, 128,   0, 143, 
	143, 139, 139,   0, 132,   0,   0,   0, 171, 174,

	145,   0, 159, 161,   0,   0,   0,   0, 132,   0, 
	  0, 143, 145,   0,   0,   0, 128,   0,   0, 134,

	145, 135, 163, 166,   0, 128, 159, 161, 147, 148, 
	149,   0, 145,   0, 139,   0, 139,   0, 132,   0,

	145,   0,   0, 138,   0,   0, 163, 166,   0,   0, 
	  0,   0, 145, 139, 141, 144, 141, 139, 139, 139,

	145, 137, 128, 159, 161, 138,   0, 144,   0, 130, 
	  0, 128, 143, 143, 143, 138, 143, 143, 143, 145,

	145,   0,   0, 163, 166,   0,   0,   0,   0, 152, 
	134,   0,   0, 142,   0,   0,   0,   0, 150, 145,

	145, 130, 131,   0, 138,   0, 128,   0, 142,   0, 
	  0, 140,   0,   0, 136, 136, 136, 136, 136, 145,

	145, 139, 139, 139, 139,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0, 136, 136, 154, 136, 136, 145,

	143, 143, 143, 143, 145, 131, 132, 139,   0,   0, 
	134, 139, 131, 130, 136, 136, 136, 136, 136, 145,

	130, 155, 157, 128, 145, 139, 139, 141, 134, 144, 
	  0, 141, 139, 139, 139, 139, 139, 139, 139, 145,

	  0, 128,   0, 132, 143, 143, 143, 143,   0, 138, 
	  0, 143, 143, 143, 143, 143, 143, 143, 143, 143,
};

location village = { 
    .portal_in={9, 10}, 
    .portal_out={0, 0}, 
    .map=&VILLAGE[0],
    .width=20,
    .height=12,
    .return_to=0 
};

container chest = {
    .position={.x=18, .y=6},
    .items={APPLE},
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
    .portal_in={.x=7, .y=4}, 
    .portal_out={.x=8, .y=7}, 
    .map=&HOUSE[0],
    .width=16,
    .height=8,
    .return_to=&village
};


void build_location_portals()
{
    village.portals[0] = &house;
    
    village.containers[0] = &chest;
    //village.containers[0]->items[0] = 102;
}

#endif