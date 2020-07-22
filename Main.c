// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Version 
// 2.0 Production
//
//
// Include Files
#include <p33FJ128GP802.h>
#include "main.h"           // Main program definitions
#include <dsp.h>			// DSP Library - note the required addition of dspli-elf library file in Project window
#include <math.h>
#include <libq.h>			// Fixed Point library		
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>       // Delays
#include <xc.h>

extern  void flash_operations(void);    // EEPROM emulation routines
extern void af_processor(void);        // Subrate audio processes

#include "eeprom.c"                     // EEPROM routines
#include "delays.c"                     // Delay routines
#include "spi_bus.c"                    // MCP6S21 PGA and MCP 4251 Pot Control

#include "carrier.c"                    //Carrier generator
#include "filters.c"                    // Filter configuration routines
#include "encoder.c"                    // Encoder inputs
#include "af_gain.c"                    // Audio gain routines
#include "analogues.c"                  // Analogue peripherals
#include "settings.c"                   // Setup routines
#include "tones.c"                      // Signal tones
#include "trx_routines.c"               // PTT line and TRX routines
#include "switches.c"                   // Mode switch routines
#include "controls_fsm.c"               // Control finite state machine

#include "isr.c"                        // ISR
//extern void fir_dec(void);            // Decimation filter assembler file

//void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt(void); // ISR Prototype

void __attribute__((interrupt, no_auto_psv))_DAC1LInterrupt(void);  // DAC ISR Prototype

#pragma config BWRP = WRPROTECT_OFF        
#pragma config SWRP = WRPROTECT_OFF         
#pragma config FNOSC = FRC        
#pragma config FCKSM = CSECMD  
#pragma config OSCIOFNC = ON        
#pragma config POSCMD = NONE         
#pragma config GSS = OFF        
#pragma config GWRP = OFF
#pragma config FWDTEN = OFF
#pragma config WINDIS = OFF  
#pragma config FPWRT = PWR128
//#pragma config ICS = PGD2                      // Debug Port
//#pragma config JTAGEN = OFF                    // Debug Enable

#pragma config UID0 = 00000003                  // Version Number  

// Start of Main program

int main(void){
        // Configure Clock PLL prescaler, PLL postscaler, PLL divisor for 10MIPS operation with internal 7.37MHz oscillator   
    PLLFBD=41;                  // M = 43,	158.455MHz VCO divided by 43 = 3.685MHz
    CLKDIVbits.PLLPOST = 0;		// N2=2, 158.455/2 = 79.225MHz = 39MIPS // N2 = 4,	158MHz/4 = 39MHz = 19.5MIPS
    CLKDIVbits.PLLPRE = 0; 		// N1 = 2,      Fin =3.685MHz

    // Initiate clock switch to FRC with PLL
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(0x01);
    while(OSCCONbits.COSC!= 0b001);     // Wait for clock switch
    while(OSCCONbits.LOCK!=1) {};       // Wait for PLL to lock

        // Configure DAC clock source
        ACLKCON=0x0700;                 // VCO output to divider, ACLK disabled, Divide by 1

        // Configure I/O Port Directions
        TRISA=PORTA_DIR;                // Set up PORTA directions
        TRISB=PORTB_DIR;                // Set up PORTB directions
        CNPU1=PULL_UPS1;                // Enable pulls ups on inputs
        CNPU2=PULL_UPS2;                //
        PORTA=0;                        // Clear ports to set
        PORTB=0;                        // unused pins low
     
        DataEEInit();                   // Initialise EEPROM emulation
        init_spi();                     // Initialise SPI
        fs=11053;                       // Define sampling rate
        init_encoders();                // Initialise encoders
 
            SIG_LED=1;                  // Flash twice, LED on
            delay_ms(250);              // Wait
            SIG_LED=0;                  // LED off
            delay_ms(250);              // Wait       
            SIG_LED=1;                  // LED on
            delay_ms(250);              // Wait           
            SIG_LED=0;                  // LED off
     
        // Load settings and initialise
        settings_load();                // Load selected stored settings       
        init_fir();                     // Initialise filters
        set_freq(freq);                 // Set carrier frequency (Hz))
        init_adc();                     // Initialise ADC
        init_dac();                     // Initialise DAC
        init_af_gain();                 // Initialise AF gain
        init_mic_gain();                // Initialise MIC gain
        switch_init();                  // Get mode switch state
        delay_ms(500);                  // Pause to allow audio to start    
        morse(F800Hz,V);                // Send a "V"        
        morse(F800Hz,TWO);              // Send a "2"
        morse(F800Hz,DOT);              // Send a "."
        morse(F800Hz,ZERO);             // Send a "0"        

	// Main program Loop
	do{                     // Endless loop
       Idle();              // Wait for ISR
       _LATB7^=1;           // ISR Test
        encoders();         // Scan encoders
        mode_switch();      // Check switch port
        ptt_test();         // Test PTT line for change
        fsm();              // Run FSM
        }while(1);
    return 0;
}

 
