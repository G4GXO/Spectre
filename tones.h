// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Tone Keyer Definitions
//#define Idle()   {__asm__ volatile ("pwrsav #1");}              // Idle Mode command

#define F700Hz  (700UL*65535UL/11053)    // 400Hz phase increment
#define F800Hz  (800UL*65535UL/11053)    // 800Hz phase increment
#define F1000Hz (1000UL*65535UL/11053)   // 1000Hz phase increment
#define F1500Hz (1500UL*65535UL/11053)   // 1500Hz phase increment

#define T50mS   550     //
#define T100mS  1105    // (100*11053UL)/1000      // 100mS tone period
#define T150mS  1658    // (150*11053UL)/1000      // 100mS tone period
#define T300mS  3315    //

// Raised Cosine Taper used for amplitude shaping of leading and trailing edges of tones
#define Nt  80          // Sequence length
const   int tone_taper[Nt]={ 0x0000,0x000C,0x0033,0x0074,0x00CE,0x0142,0x01D0,0x0276,0x0336,0x040E,
                           0x04FE,0x0606,0x0726,0x085C,0x09AA,0x0B0D,0x0C85,0x0E13,0x0FB5,0x116A,
                           0x1332,0x150C,0x16F8,0x18F4,0x1B00,0x1D1B,0x1F44,0x217A,0x23BD,0x260B,
                           0x2864,0x2AC6,0x2D31,0x2FA3,0x321C,0x349B,0x371E,0x39A5,0x3C2E,0x3EB9,
                           0x4145,0x43D0,0x4659,0x48E0,0x4B63,0x4DE2,0x505B,0x52CD,0x5538,0x579A,
                           0x59F3,0x5C41,0x5E84,0x60BA,0x62E3,0x64FE,0x670A,0x6906,0x6AF2,0x6CCC,
                           0x6E94,0x7049,0x71EB,0x7379,0x74F1,0x7654,0x77A2,0x78D8,0x79F8,0x7B00,
                           0x7BF0,0x7CC8,0x7D88,0x7E2E,0x7EBC,0x7F30,0x7F8A,0x7FCB,0x7FF2,0x7FFF,
                        };

// Morse character codes (First 1 from left is start marker after which 0=dot, 1=dash)
#define A       0b00000101      // A
#define B       0b00011000      // B
#define D       0b00001100      // D
#define E       0b00000010      // E
#define F       0b00010010      // F
#define G       0b00001110      // G
#define I       0b00000100      // I
#define K       0b00001101      // K
#define L       0b00010100      // L
#define M       0b00000111      // M
#define P       0b00010110      // P
#define Q       0b00011101      // Q
#define S       0b00001000      // S
#define T       0b00000011      // T
#define U       0b00001001      // U
#define V       0b00010001      // V
#define ZERO    0b00111111      // 0
#define ONE     0b00101111      // 1
#define TWO     0b00100111      // 2
#define THREE   0b00100011      // 3
#define DOT     0b01010101      // . (RK)
