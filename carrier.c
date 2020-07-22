// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Carrier Oscillator
//

unsigned int fs;                                       // Sampling rate
unsigned int freq;                                     // Operating frequency
extern unsigned int carrier_phase_inc;                 // Carrier Oscillator phase increment
extern volatile unsigned int carrier_phase_word;       // Carrier Oscillator phase increment
extern int carrier_out_rx;                             // Carrier output for frequency measurement in Rx
extern int carrier_out_tx;                             // Carrier output with mute for frequency measurement in Tx

// Values for Taylor series sin polynomial calculation (EMRFD Ch10.9)
// Dummy value used for last coefficient to fit with computation loop
int sin_coeff[6]={0x3240, 0x0053, 0xAACC, 0x08B7, 0x1CCE, 0x0000};

// Set carrier oscillator frequency
//
// Calculate the phase word for the carrier oscillator to achieve the required
// operating frequency. 
void set_freq(freq){
    unsigned long carrier_freq;
    
    carrier_freq=freq;
    carrier_phase_inc=((carrier_freq<<16)/fs);
}

