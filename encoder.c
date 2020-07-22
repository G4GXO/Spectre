// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Encoder Routines
//
// (c) Cumbria Designs
//
// Channel assignment
//
// The encoder channel comprises of two bits, in the examples below the encoder is
// connected to bits 0 and 1. For clarity only the associated byte is shown.
//
// Detection
//
// First determine if the encoder has changed by XORing the new value of the encoder
// word with the old value. A non zero result indicates a change of encoder state.
//
// e.g.  New          00000010
//       Old          00000011
//       XOR          00000001
//
// The direction of change is now determined by shifting the new  encoder bit pair
// one bit left, XORing it with the historic (last) state and testing the central
// bit of the three bit result.
//
//  e.g. New           00000010
//       Old           00000011
//       Left Shift    00000100
//       XOR           00000101
//
// Bit 1 conveys the direction, in this case the result is zero, rotation
// in the opposite direction would produce a 1. In the software a 1 caused an increment
// in encoder count, a zero a decrement. The encoder is wired to suit direction law.
//
// The outputs from the encoder processes are three flags whose normal state is 0
// and operated state is 1;
//
// enc_up           Clockwise direction detected
// enc_down         Anticlockwise direction detected
// enc_sw_flag      Switch has been operated
//
// All three flags must be cleared by the program that uses them.
//

#include "encoder.h"            // Encoder definitions


unsigned int    encoder_new;    // Current encoder state
unsigned int    encoder_last;   // Previous encoder state

unsigned int    enc_press;      // Encoder switch press flag
unsigned int    enc_press_rel;  // Encoder switch press release
unsigned int    enc_hold;       // Encoder hold flag
unsigned int    enc_hold_rel;   // Encoder hold release

// Memory flags to record an encoder action for later use by the release
// flags when the encoder switch returns to normal. This allows the
// last state to be stored even if the operating flags are cleared by
// the utilising function (FSM in this case). Once the encoder switch
// is released, the memory flag states are copied into the release flags.

unsigned int    enc_rate;       // Counter to run encoder checks below ISR rate to reduce false states due to contact bounce

unsigned int enc_press_mem;     // Press memory flag
unsigned int enc_hold_mem;      // Hold memory flag

unsigned int    enc_sw_count;   // Switch operated counter
unsigned int    enc_dir;        // Direction flag
unsigned int    enc_dir_last;   // Historic direction flag
unsigned int    enc_up;         // Encoder up flag
unsigned int    enc_down;       // Encoder down flag
unsigned int    enc_rate;       // Counter to run encoder checks below ISR rate to reduce false states due to contact bounce

// Rotary Encoder

void encoders(){
    // Encoder scan rate counter
    enc_rate--;                  // Decrement 1 in N rate counter
    if(!enc_rate){               // If enc_rate is at zero reload counter and run encoder processes
        enc_rate=ENCSCAN_RATE;   // Reload enc_rate counter
    //
    // Run encoder processes
    //
    // Encoder switch    
        if(!ENC_SW){
        if(enc_sw_count<HOLD_COUNT){                // If hold count not reached
            enc_sw_count++;                             // Increment press and hold counter
            if(!enc_press_mem){                     // If this is the first pass
            enc_press=1;      }                     // Set press flag
            enc_press_mem=1;                        // Set press memory flag
            }
        else {                                      // Hold count reached
            if(!enc_hold_mem){                      // If this is the first pass
            enc_hold=1;}                            // Set momentary hold flag
            enc_hold_mem=1;                         // Set hold memory flag
            enc_press=0;                            // Clear press flag
            enc_press_mem=0;                        // Clear press memory flag
            }
    }
    else{                                           // Switch released, reload press counter
        // OR in previous states to prevent erasure by clear commands on next pass.
        // Flag states are cleared by any function making use of these flags
        enc_sw_count=0;                             // Zero press and hold counter
        enc_press_rel=enc_press_mem;                // Copy press release state
        enc_hold_rel=enc_hold_mem;                  // Copy hold release state
        enc_press_mem=0;                            // Clear press memory flag
        enc_hold_mem=0;                             // Clear hold memory flag
        }    
         
    // Rotary encoder section
    // Look for an encoder change. To avoid false stepping only act upon consecutive changes in
    // the same direction. Output from the encoder is a step count value. The maximum and
    // minimum values of the step range are defined by the values held in enc_max and enc_min.

    encoder_new=PORTA&ENC_MASK;                                 // Get current encoder state
    if(encoder_new^encoder_last){                               // Has it changed?
        enc_dir=((encoder_new<<1)^encoder_last)&ENC_DIR_BIT;    // Yes, get direction bit
        if(enc_dir==enc_dir_last){                              // Is change same as previous direction?
            if(!encoder_new){                                   // Slow rate, only step on both channels zero
            if(enc_dir){                                        // Is direction bit=1?
                    enc_up=0;                                   // Yes, decrement
                    enc_down=1;
            }
            else{                                               // Direction bit =0
                    enc_down=0;                                 // Yes, increment
                    enc_up=1;
            }
            }
        }                                                       // Exit here for direction change or no change
        enc_dir_last=enc_dir;                                   // Update historic direction flag
        encoder_last=encoder_new;                               // Update historic encoder state
        }
    
    }
}
// Prepare encoder for start up with no false steps
//
void init_encoders(){
    enc_rate=1;                              // Load rate counter with 1 to force scan on first call

    encoder_new=PORTA&ENC_MASK;             // Get current encoder state
    encoder_last=encoder_new;               // Copy to old and new states
    enc_up=0;                               // Clear up flag
    enc_down=0;                             // Clear down flag
    enc_press=0;                            // Clear press flag
    enc_press_mem=0;                        // Clear press memory flag
    enc_press_rel=0;                        // Clear press release flag
    enc_hold=0;                             // Clear hold flag
    enc_hold_mem=0;                         // Clear hold memory flag
    enc_hold_rel=0;                         // Clear hold release flag
    enc_sw_count=0;                         // Zero encoder switch counter
}

