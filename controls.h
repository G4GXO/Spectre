// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Control Definitions
//
//
//

// Audio output routing for ISR
#define RX_NORMAL                   0x0000               // Rx normal AF Output
#define RX_INVERTED                 0x0001               // Rx inverted AF output
#define RX_NORMAL_TONE              0x0010               // Rx tone output normal mode
#define RX_INVERTED_TONE            0x0011               // Rx tone output inverted mode

#define TX_NORMAL                   0x0100               // Rx normal AF Output
#define TX_INVERTED                 0x0101               // Rx inverted AF output
#define TX_NORMAL_TONE              0x0110               // Rx tone output normal mode
#define TX_INVERTED_TONE            0x0111               // Rx tone output inverted mode

#define RX_ROUTING                  0xfeff               // Clear bit for Rx
#define TX_ROUTING                  0x0100               // Set bit for Tx

// Test and Setup Routes
#define CARRIER_ON                  0x1000               // Enable carrier on secondary output for frequency measurement 
#define CARRIER_OFF                 0xefff               // Disable carrier output 

#define RX_CARRIER_NORMAL           0x1000               // Rx normal with carrier on Tx output 
#define RX_CARRIER_INVERTED         0x1001               // Rx inverted with carrier on Tx output
#define RX_CARRIER_NORMAL_TONE      0x1010               // Rx normal with carrier on Tx output 
#define RX_CARRIER_INVERTED_TONE    0x1011               // Rx inverted with carrier on Tx output

#define TX_CARRIER_NORMAL           0x1100               // Tx normal with carrier on sidetone output 
#define TX_CARRIER_INVERTED         0x1101               // Tx inverted with carrier on sidetone output
#define TX_CARRIER_NORMAL_TONE      0x1110               // Tx normal with carrier on sidetone output 
#define TX_CARRIER_INVERTED_TONE    0x1111               // Tx inverted with carrier on sidetone output


