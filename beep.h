#ifndef BEEP_H
# define BEEP_H

#define SND   0

// http://www.soundoctor.com/freq.htm

//      NOTE    PERIOD   FREQ.  Note        
#define _A4    2270/2 // 440hz, Concert A(4)  0
#define _Bb4   2100/2 // 466hz, Bb(4)         1
#define _B4    2000/2 // 494hz, B(4)          2
#define _C5    1900/2 // 523hz, C(5)          3
#define _Cs5   1800/2 // 554hz, C#(5)         4
#define _D5    1700/2 // 587hz, D(5)          5
#define _Ds5   1600/2 // 622hz, D#(5)         6
#define _E5    1500/2 // 659hz, E(5)          7
#define _F5    1400/2 // 698hz, F(5)          8
#define _Fs5   1350/2 // 740hz, F#(5)         9
#define _G5    1280/2 // 748hz, G(5)          A
#define _Gs5   1200/2 // 831hz, G#(5)         B
#define _A5    1140/2 // 800 A(5)             C
#define _Bb5   1070/2 // 932, Bb(5)           D
#define _B5    1010/2 // 988, B(5)            E
#define _A8    142/2  // 7040                 F
#define _A9    71/2  // 14080hz             X

void crap_beep(unsigned int note, unsigned int dur);

#endif