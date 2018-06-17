#ifndef MAPS_H
# define MAPS_H

#include "utils.h"
#include "mobs.h"

#define MAX_PORTALS 8
#define MAX_CONTAINERS MAX_MOBS+2
#define CONTAINER_SIZE 8

//#define NULL_CONTAINER (container){.glyph=0}

typedef struct container {
    byte glyph;
    point position;
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


	 43,  43,  43,  43,  43,  43,  47,  47,  47,  43, 
	 43,  37,  34,   0,   0,   0,  32,   0,  71,  73,

	 49,  47,  47,  47,  47,  47,  37,  32,   0,  47, 
	 47,  43,  43,   0,  36,   0,   0,   0,  75,  78,

	 49,   0,  63,  65,   0,   0,   0,   0,  36,   0, 
	  0,  47,  49,   0,   0,   0,  32,   0,   0,  38,

	 49,  39,  67,  70,   0,  32,  63,  65,  51,  52, 
	 53,   0,  49,   0,  43,   0,  43,   0,  36,   0,

	 49,   0,   0,  42,   0,   0,  67,  70,   0,   0, 
	  0,   0,  49,  43,  45,  48,  45,  43,  43,  43,

	 49,  41,  32,  63,  65,  42,   0,  48,   0,  34, 
	  0,  32,  47,  47,  47,  42,  47,  47,  47,  49,

	 49,   0,   0,  67,  70,   0,   0,   0,   0,  56, 
	 38,   0,   0,  46,   0,   0,   0,   0,   0,  49,

	 49,  34,  35,   0,  42,   0,  32,   0,  46,   0, 
	  0,  44,   0,   0,  40,  40,  40,  40,  40,  49,

	 49,  43,  43,  43,  43,   0,   0,   0,   0,   0, 
	  0,   0,   0,   0,  40,  40,  58,  40,  40,  49,

	 47,  47,  47,  47,  49,  35,  36,  43,   0,   0, 
	 38,  43,  35,  34,  40,  40,  40,  40,  40,  49,

	 34,  59,  61,  32,  49,  43,  43,  45,  38,  48, 
	  0,  45,  43,  43,  43,  43,  43,  43,  43,  49,

	  0,  32,   0,  36,  47,  47,  47,  47,   0,  42, 
      0,  47,  47,  47,  47,  47,  47,  47,  47,  47,
};

location village = { 
    .portal_in={9, 10}, 
    .portal_out={0, 0}, 
    .map=&VILLAGE[0],
    .width=20,
    .height=12,
    .return_to=0 
};


static const byte HOUSE[] PROGMEM = {
      0,   0,   0,   0,   79,  85,  85,  85,  85,  85,  85,  85,   0,   0,   0,   0,   
      0,   0,   0,   0,   81,  62,   0,  69,   0,  66,   0,  81,   0,   0,   0,   0,   
      0,   0,   0,   0,   81,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,  
      0,   0,   0,   0,   81,   0,   0,   0,  86,   0,   0,  81,   0,   0,   0,   0,
      0,   0,   0,   0,   81,  77,   0,   0,  87,  88,   0,  81,   0,   0,   0,   0,
      0,   0,   0,   0,   81,  93,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   
      0,   0,   0,   0,   81,   0,   0,   0,   0,   0,   0,  81,   0,   0,   0,   0,   
      0,   0,   0,   0,   83,  85,  85,  85,   0,  85,  85,  83,   0,   0,   0,   0,
};

location house1 = { 
    .portal_in={.x=7, .y=4}, 
    .portal_out={.x=8, .y=7}, 
    .map=&HOUSE[0],
    .width=16,
    .height=8,
    .return_to=&village
};

location house2 = { 
    .portal_in={.x=3, .y=3}, 
    .portal_out={.x=8, .y=7}, 
    .map=&HOUSE[0],
    .width=16,
    .height=8,
    .return_to=&village
};

location house3 = { 
    .portal_in={.x=4, .y=6}, 
    .portal_out={.x=8, .y=7}, 
    .map=&HOUSE[0],
    .width=16,
    .height=8,
    .return_to=&village
};

void build_location_portals()
{
    village.portals[0] = &house1;
    village.portals[1] = &house2;
    village.portals[2] = &house3;
}

#endif