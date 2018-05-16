#include <avr/io.h>

#include "beep.h"
#include "utils.h"

/* Note this doesn't work or do anything, just holding it for a friend 
    until millis() gets implemented.
    
    https://www.avrfreaks.net/forum/tut-c-newbies-guide-avr-timers?name=PNphpBB2&file=viewtopic&t=50106
*/

void crap_beep(unsigned int pin, unsigned int note, unsigned int dur)
{
    DDRB |= (1<<SND);
    
    unsigned long ts = millis();
    while (millis() < ts + dur)
    {
        PORTB |= 1 << pin;    // HIGH
        delay_us(note);
        PORTB &= ~(1 << pin); // LOW
        delay_us(note);
    }
}