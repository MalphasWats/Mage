#include <avr/io.h>

#include "beep.h"
#include "utils.h"

void crap_beep(unsigned int pin, unsigned int note, unsigned int dur)
{
    DDRB |= (1<<SND);
    
    unsigned int ts = millis();
    while (millis() < ts + dur)
    {
        PORTB |= 1 << pin;    // HIGH
        delay_us(note);
        PORTB &= ~(1 << pin); // LOW
        delay_us(note);
    }
}