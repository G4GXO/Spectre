// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Encoder Definitions
//
// (c) Cumbria Designs
//
// Port A
//
// RA0  
// RA1  
// RA2  ENC CH A
// RA3  ENC CH B
// RA4  PTT
// RA5  
// RA6
//
// Port B
// RB4 ENC SW
//

#define ENCSCAN_RATE    10              // Run encoder processes once every 10 ISRs to debounce
#define HOLD_COUNT      500             // Count value for normal press and hold
#define LONG_COUNT      1500            // Long hold count
#define VLONG_COUNT     2000            // Very long hold count

#define ENC_SW        PORTBbits.RB4     // Encoder switch
#define ENC_MASK      0x000c            // Encoder channel mask (RA2 and RA3)
#define ENC_DIR_BIT   0x0008            // Direction bit
