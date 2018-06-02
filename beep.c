#include <avr/io.h>

#include "beep.h"
#include "utils.h"

void crap_beep(unsigned int note, unsigned int dur)
{
    DDRB |= (1<<SND);
    
    unsigned int ts = millis();
    while (millis() < ts + dur)
    {
        PORTB |= 1 << SND;    // HIGH
        delay_us(note);
        PORTB &= ~(1 << SND); // LOW
        delay_us(note);
    }
}

void click( void )
{
    crap_beep(_A9, 15);
}