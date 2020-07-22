// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// ISR
//
// The ISR is triggered at the ADC sampling rate of 128kHz. Within the ISR, a new sample is
// captured, decimation filtering takes place and rate is reduced to the 8kHz sampling rate.
// On each 8kHz branch the DAC input registers are updated.


volatile signed int  AF_in;                     // Raw AF input sample
volatile extern signed int  AF_norm_out;        // AF normal output sample
volatile extern signed int  AF_inv_out;         // AF inverted output sample
volatile extern signed int  mic_audio;          // Gain adjusted microphone sample
volatile extern signed int  mic_in;             // Mic input sample
volatile extern signed int  sidetone_audio;     // Tx sidetone audio sample
volatile extern signed int  tone_out;           // Tone gen output
volatile unsigned int isr_count;                // ISR counter to set 8kHz rate for sampling and filtering
volatile unsigned int subrate_flag;             // Subrate processing flag
unsigned int overload;                          // Input warning LED timer


void __attribute__((__interrupt__, auto_psv)) _DAC1LInterrupt(void)
{
   AD1CON1bits.SAMP=0;                          // Stop sampling and trigger conversion
   while (!AD1CON1bits.DONE);                   // Check that conversion has completed
   fir_in=ADCBUF0;                              // Copy sample to FIR filter input
   AF_in=ADCBUF0;                               // Copy to AF_in for sidetone
   mic_in=ADCBUF0;                              // Copy to mic in for Tx

   AD1CON1bits.SAMP=1;                          // Start sampling for next ISR
 
    // Signal detector
    if(fir_in>CLIPPING){        // If we exceed input threshold, load overload counter
       overload=1000;           // Load "monostable"
    }
    if(overload){
        --overload;             // Decrement counter
       SIG_LED=1;               // Illuminate Signal LED
        }
    else{                       // Otherwise signal is low
       SIG_LED=0;               // extinguish LED
       }   
  
        IFS4bits.DAC1LIF=0;     // Clear DAC Interrupt 
                                // Perform audio processing     
    // Tone Keyer
    //
    // When the tone_period is non zero the Tone Keyer applies raised cosine shaping stepping through
    // shaping table values pointed to by index ti. At full amplitude the tone remains on until the
    // period counter reaches zero after which a lead out taper is applied. the tone frequency is set
    // by the tone_phase_increment value passed to the assembler routine by the calling function.    
 

    if(tone_period){                                // If tone on, generate shaped tone output
        if(ti<Nt){                                  // Are we entering a tone element?
            tone_shaping=tone_taper[ti];            // Yes, generate entry taper starting at zero ending at Nt-1
            ti++;                                   // Increment index, max=Nt
            }
        tone_period--;                              // Decrement period count
        }
    else{                                           // Key is released, generate exit taper
        if(ti>0){                                   // Are we exiting a code element?   
            ti--;                                   // Yes, decrement index
            tone_shaping=tone_taper[ti];            // Apply trailing taper (work backwards through table) starting with Nt-1
            }
        }         
        
   // Left ch=Tx audio, Right ch=Rx audio
   //
   switch (af_routing){                             
       case RX_NORMAL:
           fir_in=ADCBUF0;                         // Copy sample to FIR filter input           
           DAC1RDAT=AF_norm_out;                   // AF out to Rx channel
           DAC1LDAT=0;                             // Stuff left channel with zeros for interrupt trigger  
           break;
        
       case RX_INVERTED:
           fir_in=ADCBUF0;                         // Copy sample to FIR filter input           
           DAC1RDAT=AF_inv_out;                    // AF out to Rx channel
           DAC1LDAT=0;                             // Stuff left channel with zeros for interrupt trigger            
           break;
           
       case RX_NORMAL_TONE:
           DAC1RDAT=tone_out;                      // AF out to Rx channel
           DAC1LDAT=0;                             // Stuff left channel with zeros for interrupt trigger             
           break;
           
       case RX_INVERTED_TONE:
           DAC1RDAT=tone_out;                      // AF out to Rx channel
           DAC1LDAT=0;                             // Stuff left channel with zeros for interrupt trigger
           break;

       case TX_NORMAL:
           mic_in=ADCBUF0;                         // Copy to mic in for Tx           
           fir_in=mic_audio;                       // Copy gain adjusted sample to FIR filter input           
           DAC1LDAT=AF_norm_out;                   // AF out to Tx channels
           DAC1RDAT=sidetone_audio;           
           break;
        
       case TX_INVERTED:
           mic_in=ADCBUF0;                         // Copy to mic in for Tx           
           fir_in=mic_audio;                       // Copy gain adjusted sample to FIR filter input           
           DAC1LDAT=AF_inv_out;                    // AF out to Tx channels
           DAC1RDAT=sidetone_audio;           
           break;
           
       case TX_NORMAL_TONE:
           DAC1RDAT=tone_out;                      // AF out to Tx channels             
           DAC1LDAT=0;                             // Stuff left channel with zeros for interrupt trigger
           break;
           
       case TX_INVERTED_TONE:
           DAC1RDAT=tone_out;                      // AF out to Tx channels                        
           DAC1LDAT=0;                             // Stuff left channel with zeros for interrupt trigger 
           break;

        // Carrier Setup States  to allow carrier frequency to be monitored or measured on secondary channel   
       case RX_CARRIER_NORMAL:
           fir_in=ADCBUF0;                         // Copy sample to FIR filter input           
           DAC1RDAT=AF_norm_out;                   // AF out to Rx channel
           DAC1LDAT=carrier_out_rx;                // Carrier to left channel         
           break;

       case RX_CARRIER_INVERTED:
           fir_in=ADCBUF0;                         // Copy sample to FIR filter input           
           DAC1RDAT=AF_inv_out;                    // AF out to Rx channel
           DAC1LDAT=carrier_out_rx;                // Carrier to left channel         
           break;

       case RX_CARRIER_NORMAL_TONE:
           DAC1RDAT=tone_out;                      // AF out to Rx channel
           DAC1LDAT=0;                             // Stuff left channel with zeros for interrupt trigger             
           break;
           
       case RX_CARRIER_INVERTED_TONE:
           DAC1RDAT=tone_out;                      // AF out to Rx channel
           DAC1LDAT=0;                             // Stuff left channel with zeros for interrupt trigger
           break;           
           
       case TX_CARRIER_NORMAL:
           mic_in=ADCBUF0;                         // Copy to mic in for Tx           
           fir_in=mic_audio;                       // Copy gain adjusted sample to FIR filter input           
           DAC1LDAT=AF_norm_out;                   // AF out to Tx channels
           DAC1RDAT=carrier_out_tx>>3;             // Drop audio by 18dB (sidetone gain and mute will also be applied)
           break;
        
       case TX_CARRIER_INVERTED:
           mic_in=ADCBUF0;                         // Copy to mic in for Tx           
           fir_in=mic_audio;                       // Copy gain adjusted sample to FIR filter input           
           DAC1LDAT=AF_inv_out;                    // AF out to Tx channels
           DAC1RDAT=carrier_out_tx>>3;             // Drop audio by 18dB (sidetone gain and mute will also be applied)
           break;

       case TX_CARRIER_NORMAL_TONE:
           DAC1RDAT=tone_out;                      // AF out to Tx channels             
           DAC1LDAT=0;                             // Stuff left channel with zeros for interrupt trigger
           break;
           
       case TX_CARRIER_INVERTED_TONE:
           DAC1RDAT=tone_out;                      // AF out to Tx channels                        
           DAC1LDAT=0;                             // Stuff left channel with zeros for interrupt trigger 
           break;           
    }
    af_processor();
}
