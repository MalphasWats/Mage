#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "utils.h"

unsigned int _millis = 0;

unsigned int rngSEED = 5;
unsigned int rng( void )
{
    rngSEED = (rngSEED*rngA +rngC) % rngM;
    return rngSEED;
}

void delay_ms( int ms )
{
   for (int i = 0; i < ms; i++)
   {
      _delay_ms(1);
   }
}

void delay_us( int us )
{
   for (int i = 0; i < us; i++)
   {
      _delay_us(1);
   }
}

/*  0000  0      1000  8
    0001  1      1001  9
    0010  2      1010  A
    0011  3      1011  B
    0100  4      1100  C
    0101  5      1101  D
    0110  6      1110  E
    0111  7      1111  F */

int analog_read(byte apin)
{
    ADMUX = 0x00 | apin;
    ADCSRA = 0xC7;
    
    while(ADCSRA & (1<<ADSC));
    
    byte low = ADCL;
    byte high = ADCH;
    
    //return (ADCH<<8) | ADCL;
    return (high << 8) | low;
}

void init_timer( void )
{
    TCCR1 = 0x86;  // CTC Mode, CK/32 prescale (250)
    TIMSK |= 0x40; // Enable OCIE1A Compare Interrupt
    
    sei();
                 //                     F_CPU   Prescale  Timer frequency (1 ms)
    OCR1A = 250; // Set compare value (8000000Hz / 32) / 1000Hz
}

ISR(TIMER1_COMPA_vect)
{
    _millis += 1;
}

unsigned int millis( void )
{
    return _millis;
}