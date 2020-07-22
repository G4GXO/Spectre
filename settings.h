// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Settings Header File
//
// Parameter default values for flash store settings are defined here. If a
// value read from flash does not fall within the corresponding limits
// then the default value is used.

// Limits
#define FREQ_MAX          3300      //
#define FREQ_MIN          2400      //
#define FREQ_DEFAULT      2900      //  
#define DAC_RATE_MAX        68      // DAC clock divider max (fs~9000Hz)   
#define DAC_RATE_MIN        43      // DAC clock divider min (fs~14000Hz)
#define DAC_RATE_DEFAULT    55      // DAC clock divider default

#define SIDETONE_MIN        0       // Minimum sidetone gain in dB
#define SIDETONE_MAX        22      // Maximum sidetone gain in dB
#define SIDETONE_DEFAULT    0       // Default sidetone

#define MIC_GAIN_MAX        35      // Max gain (+30dB))
#define MIC_GAIN_MIN        0       // Min gain (-5dB)
#define MIC_DEFAULT         5       // Default gain (0dB))

// Parameter Offsets
//
// The offsets are added to the modified base address to point to
// each parameter location within the preset group
#define EE_WORD_0           0       // Mic Gain and sidetone
#define EE_WORD_1           2       // Carrier Freq 
