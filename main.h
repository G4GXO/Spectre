// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Main Header File

// Oscillator defines for library delay functions
#define Fosc      80000000ULL  // Oscillator frequency Hz   
#define FCY       Fosc/2       // Instruction clock in Hz

// Booleans
#define ON        1            // ON state
#define OFF       0            // OFF state   

// I/O Allocations

// PORT A                           DIR             P/U             ADxPCFG
// RA0  RX AF INPUT         1                                        
// RA1  TX MIC INPUT        1       6               
// RA2  ENC CH A            1                       CN30
// RA3  ENC CH B            1                       CN29
//
// RA4  NORMAL/INVERT       1       1               CN0
//
// PORT B
// RB0  PTT LINE            1                       CN4
// RB1  SPI MCP6S21 LATCH   0       1
// RB2  SPI MCP6S21 DATA    0        
// RB3  SPI MCP6S21 CLK     0
//
// RB4  ENC SW              1                       CN1
// RB5                      0
// RB6  LED                 0       0
// RB7                      0
//
// RB8                      0
// RB9  SPI MCP4151 LATCH   0       0
// RB10 SPI MCP4151 CLK     0                    
// RB11 SPI MCP4151 DATA    0
//
// RB12                     0
// RB13 DAC RX AF O/P -     0       0
// RB14 DAC TX MIC O/P +    0    
// RB15 DAC TX MIC O/P -    0


// I/O Port directions
#define PORTA_DIR	0x001f      // Port A directions
#define PORTB_DIR	0x0011      // Port B directions
// Pull Ups on Inputs
#define PULL_UPS1   0x0013      // Pull ups (see pins in data sheet))
#define PULL_UPS2   0x6000      // 
// Indicators
#define     SIG_LED     _LATB5  // Signal level LED
//#define     SIG_LED     _LATB6  // Signal level LED
// Input Level Warning
#define     CLIPPING    0x7000  // Input clipping threshold  
// SPI
#define     SPI_P_SDO   _LATB11 // Pot SPI Data output
#define     SPI_P_SCLK  _LATB10 // Pot Clock output
#define     SPI_P_CS    _LATB9  // Pot Latch for Pot (MCP4151)

#define     SPI_M_SDO   _LATB2  // Mic SPI Data output
#define     SPI_M_SCLK  _LATB3  // Mic Clock output
#define     SPI_M_CS    _LATB1  // Mic Latch for Mic amp (MCP6S21)
// Digital Pot
#define POT_0_INI   0x000f      // AF start gain
#define POT_1_INI   0x100f      // AF start gain
// PTT Line
#define PTT PORTBbits.RB0       // PTT Input, 1=Rx, 0=Tx
// Mode Switch
#define MODE PORTAbits.RA4      // Normal/Invert switch
