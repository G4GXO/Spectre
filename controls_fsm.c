// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Finite State Machine
//
// A Function Pointer based scheme for menu navigation and parameter change

// State and Event Enumerations
//
// The enum type assigns incrementing numerical values to each item defined starting
// at zero unless otherwise specified.

     typedef enum {         // State List
        ST_ANY,          // Any state
        ST_RX,              // Rx state
        ST_TX,              // Tx state        
        ST_MIC_GAIN,        // Mic Gain Adjust 
        ST_FREQ_ADJ,        // Carrier Frequency Adjust        
        ST_SAVED,           // Save action complete        
        ST_INIT,            // Initialisation
     }states;

    typedef enum {          // Event List
        EV_ANY,             // Any event
        EV_NONE,            // No event
        EV_ENC_UP,          // Encoder up
        EV_ENC_DOWN,        // Encoder down
        EV_PRESS,           // Encoder switch press
        EV_PRESS_REL,       // Encoder switch released
        EV_HOLD,            // Press and hold event
        EV_HOLD_REL,        // Press and hold released
        EV_PTT_DOWN,        // Rx>Tx
        EV_PTT_UP,          // Tx>Rx
        EV_MODE_NORMAL,     // Normal operation
        EV_MODE_INVERT,     // Inverted operation        
        EV_WAIT,            // Dummy event used to stop fsm making decision
    }events;

// State, Event and function to call Structure
//
// Defines the structure of the parameters used to call a function

  typedef struct {
        states state;       // New state
        events event;       // New event
        int(*func)();       // Pointer to function to call
    }state_matrix;

// Current State and Event Variables
//
// Holds the current state and the returned event. The initialisation values
// are also defined here to determine where the state machine starts from


    static int state=ST_INIT;       // Initialisation
    static int event=EV_NONE;//

// Place these includes here to allow access to state list
  #include "controls.c"      //

//--------------------------------------------------------------------------
// State Transition Matrix
//
// This array is used to look up what function to call in response to a new
// event and the current state

    // Valid Operating States
   static state_matrix state_trans[]={
    // State        Event       Function
    // Operation
    {ST_RX,         EV_PTT_DOWN,    (*tx_setup)},             // Configure for Tx operation
    {ST_TX,         EV_PTT_UP,      (*rx_setup)},             // Configure for Rx operation          
    {ST_RX,         EV_MODE_NORMAL, (*af_normal)},            // Normal spectrum
    {ST_RX,         EV_MODE_INVERT, (*af_invert)},            // Inverted spectrum    
    {ST_RX,         EV_HOLD,        (*freq_adj)},             // Switch to frequency adjust
    {ST_TX,         EV_ENC_UP,      (*sidetone_inc)},         // Increase sidetone  
    {ST_TX,         EV_ENC_DOWN,    (*sidetone_dec)},         // Decrease sidetone
    {ST_TX,         EV_HOLD,        (*mic_adj)},              // Switch to mic gain adjust  

    {ST_FREQ_ADJ,   EV_HOLD,        (*exit_freq_adj)},        // Save settings and exit    
    {ST_FREQ_ADJ,   EV_ENC_UP,      (*freq_inc)},             // Increment DAC clock divider (fs reduce)
    {ST_FREQ_ADJ,   EV_ENC_DOWN,    (*freq_dec)},             // Decrement DAC clock divider (fs increase)
    {ST_FREQ_ADJ,   EV_MODE_NORMAL, (*af_normal)},            // Normal spectrum (during frequency adjustment)
    {ST_FREQ_ADJ,   EV_MODE_INVERT, (*af_invert)},            // Inverted spectrum (during frequency adjustment)
    {ST_FREQ_ADJ,   EV_PTT_DOWN,    (*freq_tx_setup)},        // PTT down, switch to Tx in freq adj mode 
    {ST_FREQ_ADJ,   EV_PTT_UP,      (*freq_rx_setup)},        // PTT down, switch to Rx in freq adj mode 
    
    {ST_MIC_GAIN,   EV_HOLD,        (*exit_mic_adj)},         // Save settings and exit     
    {ST_MIC_GAIN,   EV_ENC_UP,      (*mic_inc)},              // Increment mic gain
    {ST_MIC_GAIN,   EV_ENC_DOWN,    (*mic_dec)},              // Decrement mic gain       
    {ST_MIC_GAIN,   EV_PTT_UP,      (*rx_setup)},             // PTT released, switch to Rx         
    
    // AF Gain
    {ST_RX,         EV_ENC_UP,      (*af_gain_inc)},          // Increase AF Gain
    {ST_RX,         EV_ENC_DOWN,    (*af_gain_dec)},          // Decrease AF Gain          
    
    // Tones (No button press tone in Freq Adj due to conflict with af_routing carrier routing)
    {ST_ANY,         EV_PRESS,       (*tone_press)},          // Single bleep
//    {ST_TX,         EV_PRESS,       (*tone_press)},           // Single bleep
//    {ST_MIC_GAIN,   EV_PRESS,       (*tone_press)},           // Single bleep 
    
    // Initialisation
    {ST_INIT,       EV_NONE,        (*init_fsm)},             // Normal initialisation
    };

// Runtime calculation of state table entries (saves manual coding)
#define STATE_COUNT sizeof(state_trans)/sizeof(state_trans[0])


//--------------------------------------------------------------------------
// Get Event Routine
//
// Examines the encoder and switch. If a change is seen the corresponding event state
// is passed to the finite state machine for action. The order in which the events
// are handled is important.

int get_event(){

    // Button actions
    // Encoder B (AF Gain and Balance)
    if(enc_hold){                   // Hold active
        enc_hold=0;                 // Clear flag
        return EV_HOLD ;            // Return event
    }
    if(enc_hold_rel){               // Hold released
        enc_hold_rel=0;             // Clear flag
        return EV_HOLD_REL;         // Return event
    }
    else if(enc_press){             // Press active
        enc_press=0;                // Clear flag
        return EV_PRESS;            // Return event
    }
    else if(enc_press_rel){         // Press released
        enc_press_rel=0;            // Clear flag
        return EV_PRESS_REL;        // Return event
    }
    // Encoder rotate
    else if(enc_up){
        enc_up=0;                   // Clear flag
        return EV_ENC_UP;           // Return state
    }
    else if(enc_down){
        enc_down=0;                 // Clear flag
        return EV_ENC_DOWN;         // Return state
    }
    // PTT
    else if(ptt_down){              // Look for Rx>Tx change 
        ptt_down=0;                 // Clear flag
        return EV_PTT_DOWN;         // Return event
    }
    else if(ptt_up){                // Look for Tx>Rx change 
        ptt_up=0;                   // Clear flag
        return EV_PTT_UP;           // Return event
    }
    // MODE                         // Check normal/invert switch
    else if(mode_high){             // Check if switch is high
        mode_high=0;                // Clear flag
        return EV_MODE_NORMAL;      // Return event
    }
    else if(mode_low){              // Check if switch is low
        mode_low=0;                 // Clear flag
        return EV_MODE_INVERT;      // Return event
    }
    // The fall-through event (nothing)
    else return EV_NONE;
}

//------------------------------------------------------------------------------
// Controls Finite State Machine
//
// This short routine checks for a new event and if seen uses the State Transition
// Matrix to determine the current state and corresponding action for the new event.
//
void fsm(){
    int i;                       // Counter

    event = get_event();         // Check for new event

    // Step through each state and check for correlation with state and event
    // In addition to individual states and events an "ANY" condition is
    // included to apply an action irrespective of state or event.
    for(i=0; i<STATE_COUNT; i++){
         if((state_trans[i].state == state)||(state_trans[i].state == ST_ANY)){
             if((state_trans[i].event == event)||(state_trans[i].event == EV_ANY)){
                 // Call the function associated with transition (also returns next state)
                state = (state_trans[i].func)();
                break;
             }
         }
     }
}

