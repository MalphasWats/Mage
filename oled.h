#ifndef OLED_H
# define OLED_H

#include <avr/pgmspace.h>

#include "utils.h"

#define MSBFIRST 0
#define LSBFIRST 1

#define SCL   1
#define SDA   2
#define DC    3

#define CMD     LOW
#define DATA    HIGH

#define WIDTH   128
#define HEIGHT  64

#define SCREEN_COLUMNS 16
#define SCREEN_ROWS    8

static const byte BLANK[] PROGMEM = {0, 0, 0, 0, 0, 0, 0, 0};

void shift_out(byte val, byte order);

void shift_out_block(const byte *block);

void send_command(byte command);
void initialise_oled(void);
void clear_display(void);
void display_off(void);
void display_on(void);

void display_image(const byte *img, int col, int row, int width, int height);
void display_block(const byte *block, int col, int row);

#endif