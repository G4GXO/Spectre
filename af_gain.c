// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// AF Gain
//

#include "af_gain.h"

// 
//
int         wiper_0;                        // AF pot wiper 0 position (0..256)
int         af_gain_dB;                     // AF gain in dB
int         sidetone_dB;                    // AF sidetone in dB
extern int  AF_mute;                        // AF Mute control
int         mic_dB;                         // Gain index
extern int  mic_gain_factor;                // 1dB gain factor
extern int  sidetone_factor;                // Sidetone gain factor

void set_af_gain(int gain_dB){
     wiper_0=pot_dB_table[gain_dB];         // Load wiper position from table
     wiper_0=WIPER_0_WR|wiper_0;            // Set wiper 0
     write_pot(wiper_0);                    // Write to SPI
     if(!gain_dB){                          // If gain setting is at 0dB
         AF_mute=ON;                        // Enable hard sidetone mute
     }
     else{                                  // Otherwise
         AF_mute=OFF;                       // Mute is off 
     }
}

void init_af_gain(){                        // Soft start gain ramp up to stored setting
    af_gain_dB=6;//AF_GAIN_DEFAULT;             // Load default setting
    set_af_gain(af_gain_dB);                // and apply   
}                                           //

void set_mic_gain(int mic_dB){
    mic_gain_factor=mic_gain_table[mic_dB][1];  // Get 1dB increment gain factor
    write_mic(mic_gain_table[mic_dB][0]);       // Write 6dB step to SPI
}

void init_mic_gain(){   
      set_mic_gain(mic_dB);                 // Apply stored mic_dB   
}

void init_sidetone(){
        
}

