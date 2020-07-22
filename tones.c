// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Tones
//

extern volatile int tone_phase_inc;
extern volatile int tone_shaping;       // Envelope scaling factor
volatile int ti=0;                      // ISR counted table index
volatile int tone_period;               // Tone duration, when non zero tone is on

#include "tones.h"

// Shaped tone generator employing a raised cosine table to produce a variable duration
// tone at a frequency set by the calling routine. The routine receives the tone phase
// increment (which sets frequency) and character from the calling routine. The phase 
// increment is passed on to the assembler tone generator, the keying period is used 
// by the ISR to time the element lengths.
//
// The morse generator converts 8 bit character codes into keying sequences. The start
// beginning of a character is marked by a one after which the dot and dash elements are
// described by zeros and ones. The first task is to locate the marker bit which indicates 
// where the code elements start. After this each bit is inspected and converted into dot 
// or dash keying sequences with timing set by tone period.

void morse_generator(int phase_inc, char character){
    int tester=128;                         // Bit test pattern
    tone_phase_inc=phase_inc;               // Set up tone phase increment
    
    do{                                     // Find position of marker bit (first 1)
        tester=tester>>1;                   // Move test pattern right one bit
    }while(!(tester&character));            // Continue until a 1 is found
                                            // We exit with b and tester on the marker bit,
    tester=tester>>1;                       // Move on one bit to align with first element
    do{                                     // Work through character elements
        ti=1;                               // Start tone taper off
        if(character&tester){               // If element is a 1, send a dash
            tone_period=T150mS;             // Set dash period in ISR passes
        }
        else{                               // Character is a zero, send a dot 
            tone_period=T50mS;              // Set dot period in ISR passes
        }
     do{   
         Idle();                            // Wait for next ISR
        }while(ti);                         // Wait here for shaping to complete  
    tester=tester>>1;                       // Next bit
    delay_ms(50);                           // Inter element delay
    }while(tester);                         // Continue until test pattern shifted to zero
    delay_ms(100);                          // 3 element inter character delay (50mSec + 100mSec)
}
