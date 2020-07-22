// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Control Routines
//

#include "controls.h"               // Control definitions
volatile int af_routing;            // Output signal routing

//==============================================================================
// Tones
//
// To maintain tone frequency the DAC rate (and hence ISR and sampling rates) are
// set to the default value for the tone duration.

void tone_path(){
     af_routing=af_routing|0x0010;  // Set tone routing bit 
}

void restore_path(){
    af_routing=af_routing&0xffef;   // Clear tone routing bit 
}

int tone_press(){                   // Button press tone "E"
    int state_temp;                 // Temp store for state
    state_temp=state;               // Store current state
    tone_path();
    morse_generator(F1000Hz,E);     // Send tone
    restore_path();
    return state_temp;              // Restore state
}

void tone_limit(){                  // Limit tone "S"
    tone_path();
    morse_generator(F1000Hz,S);     // Send tone
    restore_path();
}

void morse(int tone, char character){
    tone_path();                        // Set up tone path
    morse_generator(tone,character);    // Send character
    restore_path();                     // Restore audio path   
}

//==============================================================================
// Folding Frequency Shift
//
// Change Carrier Frequency

int freq_adj(){                         // Switch to frequency adjust mode
    morse(F800Hz,F);                    // Send an F for frequency
    af_routing=af_routing|CARRIER_ON;   // Enable test output
    return  ST_FREQ_ADJ;                // Return state
}

int exit_freq_adj(){                    // Exit frequency adjust mode and save setting
    settings_store();                   // Save 
    af_routing=af_routing&CARRIER_OFF;  // Switch off carrier output
    morse(F800Hz,G);                    // Send a G for Gain
    return ST_RX;                       // Return state
}

int freq_inc(){
    freq=freq+5;                        // Increment freq
    if(freq>FREQ_MAX){                  // If limit exceeded hold at limit
        freq=FREQ_MAX;
        tone_limit();                   // Sound limit tone
    }
    set_freq(freq);                     // Calculate new phase increment    
    return ST_FREQ_ADJ;                 // Return state
}

int freq_dec(){
    freq=freq-5;                        // Decrement freq
    if(freq<FREQ_MIN){                  // If limit exceeded hold at limit
        freq=FREQ_MIN;
        tone_limit();                   // Sound limit tone        
    }    
    set_freq(freq);                     // Calculate new phase increment
    return ST_FREQ_ADJ;                 // Return state
}
//==============================================================================
// Output Spectrum Select

int af_normal(){
    int state_temp;                 // Temp store for state
    state_temp=state;               // Store current state   
    af_routing=af_routing&0xfffe;   // Clear inversion bit
    return state_temp;              // Return state    
}

int af_invert(){
    int state_temp;                 // Temp store for state 
    state_temp=state;               // Store current state    
    af_routing=af_routing|0x0001;   // Set inversion bit 
    return state_temp;              // Return state    
}

//==============================================================================
// AF Gain Routines
//

int af_gain_inc(){                      // Increase AF Gain
    af_gain_dB++;                       // Increment gain
    if(af_gain_dB>AF_GAIN_MAX){         // If range exceeded hold at upper limit
        af_gain_dB=AF_GAIN_MAX;
    }
    set_af_gain(af_gain_dB);
    return ST_RX;                       // Return state
}

int af_gain_dec(){                      // Decrease AF Gain
    af_gain_dB--;                       // Decrement gain
    if(af_gain_dB<AF_GAIN_MIN){         // If range exceeded hold at lower limit
        af_gain_dB=AF_GAIN_MIN;
    }
    set_af_gain(af_gain_dB);
    return ST_RX;                       // Return state
}
//==============================================================================
// Sidetone Gain Routines
//
int sidetone_inc(){                     // Increase sidetone
    sidetone_dB++;                      // Increment gain
    if(sidetone_dB>SIDETONE_MAX){       // If range exceeded hold at upper limit
        sidetone_dB=SIDETONE_MAX;
    }
    set_af_gain(sidetone_dB);
    return ST_TX;                       // Return state
}

int sidetone_dec(){                     // Decrease sidetone
    sidetone_dB--;                      // Decrement gain
    if(sidetone_dB<SIDETONE_MIN){       // If range exceeded hold at lower limit
        sidetone_dB=SIDETONE_MIN;
    }
    set_af_gain(sidetone_dB);
    return ST_TX;                       // Return state
}

int sidetone_save(){
    settings_store();                   // Save sidetone gain
    morse(F800Hz,S);                    // Send a S for Saved    
    return ST_TX;
}
//==============================================================================
// Mic Gain Routines
//

int mic_adj(){                          // Switch to frequency adjust mode
    morse(F800Hz,M);                    // Send an M for microphone
    return  ST_MIC_GAIN;                // Return state
}

int exit_mic_adj(){                     // Exit frequency adjust mode and save setting
    settings_store();                   // Save settings
    morse(F800Hz,S);                    // Send a S for Sidetone
    return ST_TX;                       // Return state
}

int mic_inc(){                          // Increase mic gain
    mic_dB++;                           // Increment gain
    if(mic_dB>MIC_GAIN_MAX){            // If range exceeded hold at upper limit
        mic_dB=MIC_GAIN_MAX;
    }
    set_mic_gain(mic_dB);
    return ST_MIC_GAIN;                 // Return state
}

int mic_dec(){                          // Decrease mic gain
    mic_dB--;                           // Decrement gain
    if(mic_dB<MIC_GAIN_MIN){            // If range exceeded hold at lower limit
        mic_dB=MIC_GAIN_MIN;
    }
    set_mic_gain(mic_dB);
    return ST_MIC_GAIN;                 // Return state
}

//==============================================================================
// T/R Change
//

int tx_setup(){                         // Switch to Tx operation
    AD1PCFGL=0xFFFD;                    // Pin AN1 is analogue input
    AD1CHS0=0x0001;                     // Channel input from AN1 (Mic)    
    set_af_gain(sidetone_dB);           // Set sidetone level
    af_routing=af_routing|TX_ROUTING;   // Set Tx routing bit
    return ST_TX;
}

int rx_setup(){                         // Switch to Rx operation
    AD1PCFGL=0xFFFE;                    // Pin AN0 is analogue input
    AD1CHS0=0x0000;                     // Channel input from AN0 (Rx)    
    set_af_gain(af_gain_dB);            // Set AF gain level
    af_routing=af_routing&RX_ROUTING;   // Clear Tx routing bit    
    return ST_RX;
}

int freq_tx_setup(){                    // Switch to Tx operation in freq adjust
    AD1PCFGL=0xFFFD;                    // Pin AN1 is analogue input
    AD1CHS0=0x0001;                     // Channel input from AN1 (Mic)    
    set_af_gain(sidetone_dB);           // Set sidetone level
    af_routing=af_routing|TX_ROUTING;   // Set Tx routing bit
    return ST_FREQ_ADJ;
}

int freq_rx_setup(){                    // Switch to Rx operation in freq adjust
    AD1PCFGL=0xFFFE;                    // Pin AN0 is analogue input
    AD1CHS0=0x0000;                     // Channel input from AN0 (Rx)    
    set_af_gain(af_gain_dB);            // Set AF gain level
    af_routing=af_routing&RX_ROUTING;   // Clear Tx routing bit    
    return ST_FREQ_ADJ;
}

//==============================================================================
// Initialisation Routines
//
// The encoder is re-initialised at this point to clear any flags in case we have
// arrived here from a restore action.


int init_fsm(){
    ptt_down=0;                           // Clear PTT flags  
    ptt_up=0;
    if(!MODE){                            // Apply selected mode
        af_routing=af_routing|0x0001;     // Mode input low, set inversion bit       
    }
    else{                                // Mode input high
        af_routing=af_routing&0xfffe;    // Clear inversion bit  
    }    
    return ST_RX;                         // Start in Rx  
}

