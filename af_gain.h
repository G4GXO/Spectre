// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// AF gain Definitions
#define AF_GAIN_MAX     0x0027  // Max AF gain  (38)
#define AF_GAIN_MIN     0x0000  // Min AF_Gain  (0)
#define AF_GAIN_DEFAULT 0x000A  // Initial Gain (-29dB)

// Digital Pot Definitions
#define WIPER_0_WR  0x0000  // Wiper 0 address and write command
#define WIPER_MAX   256     // Maximum wiper value
#define WIPER_MIN   0       // Minimum wiper value

// Reverse Log Digital Pot Addresses for 1dB steps, mute (0x0100) to max (0x0000)
// for hardware with Pot A terminal grounded

const int pot_dB_table[40]={0x0100,	0x00FF,	0x00FE,	0x00FD,
                            0x00FC,	0x00FB, 0x00FA, 0x00F9,
                            0x00F8,	0x00F7,	0x00F6,	0x00F5,
                            0x00F4,	0x00F3, 0x00F2, 0x00F0, 
                            0x00EE, 0x00EC, 0x00E9, 0x00E6, 
                            0x00E3, 0x00E0, 0x00DC, 0x00D7, 
                            0x00D2, 0x00CD, 0x00C7, 0x00C0, 
                            0x00B8, 0x00AF, 0x00A5, 0x009A, 
                            0x008E, 0x0080, 0x0070, 0x005E, 
                            0x004B, 0x0035, 0x001C, 0x0000,};


static const unsigned int mic_gain_table[][2]={
                    {0x4000,  0x47FA,},     // -5dB
                    {0x4000,  0x50C2,},     // -4dB
                    {0x4000,  0x5A9D,},     // -3dB
                    {0x4000,  0x65AB,},     // -2dB
                    {0x4000,  0x7213,},     // -1dB
                    {0x4000,  0x7FFF,},     // +0dB
                    
                    {0x4001,  0x47FA,},     // +1dB
                    {0x4001,  0x50C2,},     // +2dB
                    {0x4001,  0x5A9D,},     // +3dB
                    {0x4001,  0x65AB,},     // +4dB
                    {0x4001,  0x7213,},     // +5dB
                    {0x4001,  0x7FFF,},     // +6dB
                    
                    {0x4002,  0x47FA,},     // +7dB
                    {0x4002,  0x50C2,},     // +8dB
                    {0x4002,  0x5A9D,},     // +9dB
                    {0x4002,  0x65AB,},     // +10dB
                    {0x4002,  0x7213,},     // +11dB
                    {0x4002,  0x7FFF,},     // +12dB
                    
                    {0x4004,  0x47FA,},     // +13dB
                    {0x4004,  0x50C2,},     // +14dB
                    {0x4004,  0x5A9D,},     // +15dB
                    {0x4004,  0x65AB,},     // +16dB
                    {0x4004,  0x7213,},     // +17dB
                    {0x4004,  0x7FFF,},     // +18dB
                    
                    {0x4006,  0x47FA,},     // +19dB
                    {0x4006,  0x50C2,},     // +20dB
                    {0x4006,  0x5A9D,},     // +21dB
                    {0x4006,  0x65AB,},     // +22dB
                    {0x4006,  0x7213,},     // +23dB
                    {0x4006,  0x7FFF,},     // +24dB
                    
                    {0x4007,  0x47FA,},     // +25dB
                    {0x4007,  0x50C2,},     // +26dB
                    {0x4007,  0x5A9D,},     // +27dB
                    {0x4007,  0x65AB,},     // +28dB
                    {0x4007,  0x7213,},     // +29dB
                    {0x4007,  0x7FFF,},     // +30dB
};


