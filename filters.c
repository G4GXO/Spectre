// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Filter Routines

// Filter Kernel Files
#include    "filters.h"         // Definitions

//#define N	501                       // Filter delay line reserved length in WORDS (must match assembler file BYTES/2)

// FIR Filters
// Common
extern int  fir_n;                  // Filter length (common)
extern int  fir_m;                  // M=N-1
extern int	fir_coeff[Nfir];        // Coefficient table
extern int	fir_coeff_end;          // Last address of coefficients
// Input Filter
extern int	fir_in_delay[Nfir];		// Input filter delay line
extern int	fir_in_delay_end;		// Last address of tap vector
extern int 	fir_in_delay_ptr;		// Delay line pointer
// Output Filter
extern int	fir_out_delay[Nfir];	// Input filter delay line
extern int	fir_out_delay_end;		// Last address of tap vector
extern int 	fir_out_delay_ptr;		// Delay line pointer
//
extern int      fir_in;                 // Filter input
extern int      fir_out;                // Filter output

//================================================================================================
//
// Filter Functions
//
//================================================================================================
//
// Clear Filter Delay
//
// Fill delay line with zeros to clear it of data.

void fir_clear(void){
	int i;                              // Declare a counter
	for (i=0; i<Nfir; i++){             // Loop through all taps
			fir_in_delay[i]=0;          // Clear the delay line tap register at [i]
			fir_out_delay[i]=0;         // Clear the delay line tap register at [i]            
		}    
}

// Filters Initialisation Routine
void init_fir(void){ 
                                 
        // Initialise FIR Filter
        fir_n=Nfir;                                       // Filter length
        fir_m=Nfir-1;                                     // M=N-1

        fir_in_delay_ptr=(int)fir_in_delay;               // Set pointer to start position of left delay line
        fir_in_delay_end=fir_in_delay_ptr+((2*Nfir)-1);   // Calculate delay line end
        fir_out_delay_ptr=(int)fir_out_delay;             // Set pointer to start position of left delay line
        fir_out_delay_end=fir_out_delay_ptr+((2*Nfir)-1); // Calculate delay line end 

    int i;        
	for (i=0; i<Nfir; i++){                 // Loop through all taps
            fir_coeff[i]=fir_300_2700[i];   // Load common filter kernel     
	//		fir_coeff[i]=fir_300_2500[i];   // Load common filter kernel        
	//		fir_coeff[i]=fir_200_2700[i];   // Load common filter kernel 
	//		fir_coeff[i]=fir_2800[i];   // Load common filter kernel             
    }
}