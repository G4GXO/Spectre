// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Setup Routines
//
// These routines read and save settings to flash memory via the EEPROM emulation
// routines. Data is checked after each read to see if it falls within the valid
// range for the parameter. If it does it is used, if not it is replaced by a
// default value.

#include "settings.h"

// Saved parameters

//volatile int    audio_routing;      // Audio path selection within ISR
//volatile int    routing_temp;       // Store for current ISR audio routing       

// Read EEPROM data, unpack to form settings and validate against parameter limits. If
// within range use the read setting, if out of range use the default value. The base
// address is passed from the calling routine, these are defined in the settings h file
// and represent the start address of the block of data relating to the chosen address.
// Each associated parameter is accessed using an address offset.
//
// Data Format
//
//        Word 0                Word 1                   
//_____________________   ____________________ 
//|          |          |          |          |  
//| MIC GAIN |SIDETONE  |    CARRIER FREQ     |
//

void settings_load(){
    int data;
    // Mic Gain and Sidetone
    data=DataEERead(EE_WORD_0);                                 // Read first word
    sidetone_dB=data&0x00ff;                                    // Mask off SIDETONE gain byte     
    mic_dB=(data>>8)&0x00ff;                                    // Shift and mask off MIC_GAIN byte
    // Validate and use defaults if out of range
    if((sidetone_dB<SIDETONE_MIN)|(sidetone_dB>SIDETONE_MAX)){  // Check data is within range
        sidetone_dB=SIDETONE_DEFAULT;}                          // If out of range load default
    if((mic_dB<MIC_GAIN_MIN)|(mic_dB>MIC_GAIN_MAX)){            // Check data is within range
        mic_dB=MIC_DEFAULT;}                                    // If out of range load default 
    
    // Carrier Freq
    freq=DataEERead(EE_WORD_1);                                 // Read second word   
    if((freq<FREQ_MIN)|(freq>FREQ_MAX)){                        // Check data is within range
        freq=FREQ_DEFAULT;}                                     // If out of range load default     
}

// Store Current Settings
//
// Data is packed and written to current preset address
void settings_store(){
    int data=0;
    // Pack and store MIC_GAIN and SIDETONE   
    data=((mic_dB<<8)&0xff00);                          // MIC_GAIN
    data=((data)|(sidetone_dB&0x00ff));                 // OR in SIDETONE
    DataEEWrite(data,EE_WORD_0);                        // Store Word
    // store Carrier freq
    DataEEWrite(freq,EE_WORD_1);                        // Store word
}

