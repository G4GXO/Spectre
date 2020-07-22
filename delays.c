// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Delay Routines
//

// Two delay routines allow delays from 1 microsecond to 10 seconds. The delays are not precise and will
// be extended by a few machine cycles due to call, return and general intialisation times. An additional
// delay will be incurred where an interrupt occurs during a delay period.
// Use the following syntax to call a delay from within a program;
//
// delay_us(delay time in microsoeconds) e.g. delay_us(100);
// delay_ms(delay time in milliseconds) e.g. delay_ms(100);
//
// Pre-processor calculation of instruction cycle time and scaling factor to set number of
// delay loop cycles, adjust FC to match processor clock

#define  FC     80000000           // Processor clock
#define  MIPS   (FC/2)             // Instruction cycles per sec
#define  DLC    (MIPS/5)           // Delay loop cycle per sec
#define  US_COUNT  (DLC/1000000)   // Delay loop cycles per usec
#define  MS_COUNT  (DLC/1000)      // Delay loop cycles per msec

// Microsecond delay routine (1 to 1000 uSec)
//
// Single loop delay which scales the required value in uSec to the number of loop
// cycles needed to produce the period.
void delay_us(unsigned int uSec){
	if((!uSec)){                // If a zero delay value is used, set delay to 1
		uSec=1;                 // to prevent a roll over count
	}

	if (uSec>1000){             // If we exceed 1000uSec
		uSec=1000;              // Hold count at 1000uSec
	}
    __delay_us(uSec);           // Library delay function    
}

// Millisecond delay routine (1 to 10,000mSec)
//
// Two loop delay using a 1000uSec inner loop and an outer loop which counts mSec.
void delay_ms(unsigned int mSec){

	if((!mSec)){				// If a zero delay value is used, set delay to 1
		mSec=1;                 // to prevent a roll over count
	}

	else if (mSec>10000){		// If we exceed 10,000mSec
		mSec=10000;             // Hold count at 10,000mSec
	}
  __delay_ms(mSec);             // Library delay function
}
