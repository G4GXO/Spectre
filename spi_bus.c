// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// SPI Interface
// 3 wire uni-directional interface comprising of Data, Clock and Latch
// Pin definitions in dsPICdsp.h file
// See MPC4151 data sheet for description and timings
// SPI Mode 0,0 Clock Idle Low

void	write_SPI_pot(int data){                // Pot SPI Routine
    // Data word passed in w0           
    asm("loop_spi_pot:");                       // Loop start address
        SPI_P_SDO=0;                            // Set data output low
        asm("and w0,w1,w2");                    // Test data word bit position
        asm("bra Z, shift_spi_pot");            // If zero branch to shift function
        SPI_P_SDO=1;                            // Non zero, set data output high
    asm("shift_spi_pot:");                      // Shift function loop address
        SPI_P_SCLK=1;                           // Raise Clock line to load bit
        asm("lsr w1, w1");                      // Shift bit pattern right one bit
        SPI_P_SCLK=0;                           // Drop clock line
        asm("bra NZ, loop_spi_pot");            // While bit pattern non zero go back round again
}

void	write_SPI_mic(int data){                // Mic SPI Routine
    // Data word passed in w0
    asm("loop_spi_mic:");                       // Loop start address
        SPI_M_SDO=0;                            // Set data output low
        asm("and w0,w1,w2");                    // Test data word bit position
        asm("bra Z, shift_spi_mic");            // If zero branch to shift function
        SPI_M_SDO=1;                            // Non zero, set data output high
    asm("shift_spi_mic:");                      // Shift function loop address
        SPI_M_SCLK=1;                           // Raise Clock line to load bit
        asm("lsr w1, w1");                      // Shift bit pattern right one bit
        SPI_M_SCLK=0;                           // Drop clock line
        asm("bra NZ, loop_spi_mic");            // While bit pattern non zero go back round again
}


void	write_pot(int wiper){                   // Write wiper position to MCP4151
    asm("mov #0x8000, w1");                     // Load bit test pattern into w1
    SPI_P_SCLK=0;                               // Clock Low
    SPI_P_CS=0;                                 // Latch Low
    write_SPI_pot(wiper);                        //
    SPI_P_CS=1;                                 // Raise latch line to instruct transfer of word
}

void	write_mic(int mic_dB){                  // Write gain step to MCP6S21
    asm("mov #0x8000, w1");                     // Load bit test pattern into w1
    SPI_M_SCLK=0;                               // Clock Low
    SPI_M_CS=0;                                 // Latch Low
    write_SPI_mic(0x4000|mic_dB);               // Send
    SPI_M_CS=1;                                 // Raise latch line to instruct transfer of word
}

// initialise for SPI Mode 0,0
void    init_spi(){
    SPI_P_SCLK=0;                               // Clock Idle low
    SPI_P_CS=1;                                 // Chip Select high
    SPI_M_SCLK=0;                               // Clock Idle low
    SPI_M_CS=1;                                 // Chip Select high
}
