#ifndef UTILS_H
# define UTILS_H

#define ever ;;

#define LOW 0
#define HIGH 1

#define TRUE 0xFF
#define FALSE 0x00

#define ADC0 0  // PB5
#define ADC1 1  // PB2
#define ADC2 2  // PB4
#define ADC3 3  // PB3

typedef unsigned char byte;

typedef struct point {
    int x;
    int y;
} point;

void delay_ms( int ms );
void delay_us( int us );


// http://www.oz4.us/2015/12/recoding-bare-millis-clock-on-avr.html
// https://github.com/sourceperl/millis/blob/master/millis.c
// https://www.avrfreaks.net/forum/tut-c-newbies-guide-avr-timers?page=all
void init_timer( void );
unsigned int millis( void );

//https://github.com/SpenceKonde/ATTinyCore/blob/master/avr/cores/tiny/wiring_analog.c
//http://www.technoblogy.com/show?TWD
int analog_read(byte apin);

#endif