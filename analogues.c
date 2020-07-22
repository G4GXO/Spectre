// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Internal Analogue Peripherals Definitions and Set Up Routines
//
//
//
int dac_rate;       // DAC_DIV value to set sampling rate

// The ADC resolution is increased from 12 bits to 14 bits through oversampling at 128kHz and decimation.
// The conversion requires 14 TAD clock cycles. Minimum TAD period is 75nSec, the system clock is at 10MIPs
// giving an instruction period of 100nSEc. Hence the ADCS bit value for the ADC clock divider is;
//
// ADCS=(75nSec/25nSec)-1 = 2
//
// Configure ADC

void init_adc(){
    AD1CON1=0x0000;                 // Module off
    AD1PCFGL=0xFFFC;                // Pins AN0 and AN1 are analogue inputs
    AD1CHS0=0x0000;                 // Channel input from AN0 (Rx)
    AD1CON2=0x0000;                 // Vdd/Vss voltage reference, ch0,
    AD1CON3=0x0002;                 // System conversion clock, no autosample set, ADCS=(75E^-9*40MIPS)-1
    AD1CON1bits.AD12B=1;            // 12 bit operation
    AD1CON1bits.FORM=3;             // Signed fractional
    AD1CON1bits.SSRC=0;             // Clearing sample bit starts conversion    
    AD1CON1bits.ADON=1;             // Module on
    AD1CON1bits.SAMP=1;             // Start sampling for next ISR
}                                   // ADC ready for operation

// DAC Interrupts
//
// Interrupt is driven by Right Channel FIFO empty. This causes the sample held in AF_out to be
// loaded into the DAC left and right data registers and triggers a new ADC sample and filtering
// process to refresh AF_out for the next interrupt. The FIFO is 4 samples deep to provide buffering
// so when the interrupt occurs there are three samples in the buffer. The interrupt tops up the
// FIFO to 4 samples.

// Configure DAC

void init_dac(){
    DAC1STATbits.ROEN = 1;          // Right Channel DAC Output ON
    DAC1STATbits.LOEN = 1;          // Left Channel DAC Output ON
    DAC1STATbits.RITYPE = 0;        // Right Channel Interrupt if FIFO is not full
    IPC19bits.DAC1RIP=0;            // Right Channel Interrupt Priority (disabled)
    DAC1STATbits.LITYPE = 1;        // Left Channel Interrupt if FIFO is not Full
    IPC19bits.DAC1LIP=7;            // Left Channel Interrupt Priority (highest)

    DAC1CONbits.AMPON = 0;          // Amplifier on during Sleep and Idle Modes

    // DAC VCO source is at 158.455MHz
    // DACFDIV=158.455x10^6/(256*fs)
    // 
    // (Assumes 158.455MHz system clock divided by 2 - data sheet error!)
    DAC1CONbits.DACFDIV = 55; //n=56 fs=11,053 at 70MIPS //27;       //n=28 fs=11,053    18;        //n=19,fs=16244Hz27;
    //DAC1CONbits.DACFDIV = 37; //n=38 fs=8,144
                                    
    DAC1CONbits.FORM = 1;           // Data Format is Signed Integer
    DAC1DFLT = 0x0000;              // Default value set to Midpoint when FORM = 1
    IFS4bits.DAC1RIF = 0;           // Clear Right Channel Interrupt Flag
    IFS4bits.DAC1LIF = 0;           // Clear Left Channel Interrupt Flag
    IEC4bits.DAC1RIE = 0;           // Right Channel Interrupt Disabled
    IEC4bits.DAC1LIE = 1;           // Left Channel Interrupt Enabled        
    
    DAC1RDAT=0x4000;                // Fill FIFO Buffer
    DAC1RDAT=0x4000;
    DAC1RDAT=0x4000;
    DAC1RDAT=0x4000;

    DAC1LDAT=0x4000;                // Fill FIFO Buffer
    DAC1LDAT=0x4000;
    DAC1LDAT=0x4000;
    DAC1LDAT=0x4000;

    DAC1CONbits.DACEN = 1;          // DAC1 Module Enabled
}

void interrupt_enable(){
    IFS4bits.DAC1RIF = 0;           // Clear Right Channel Interrupt Flag
    IFS4bits.DAC1LIF = 0;           // Clear Left Channel Interrupt Flag
    IEC4bits.DAC1RIE = 0;           // Right Channel Interrupt Disabled
    IEC4bits.DAC1LIE = 1;           // Left Channel Interrupt Enabled
}

void interrupt_disable(){
    IFS4bits.DAC1RIF = 0;           // Clear Right Channel Interrupt Flag
    IFS4bits.DAC1LIF = 0;           // Clear Left Channel Interrupt Flag
    IEC4bits.DAC1RIE = 0;           // Right Channel Interrupt Disabled
    IEC4bits.DAC1LIE = 0;           // Left Channel Interrupt Disabled    
}



