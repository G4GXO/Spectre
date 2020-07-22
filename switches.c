// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// Switch routines
//

int mode_high;      // Mode flag, high=normal spectrum
int mode_low;       // Mode flag, low=inverted spectrum
int mode_last;      // Previous switch state

void mode_switch(){
    if((MODE^mode_last)){      // Has the switch state changed?
        if(!MODE){              // Yes, is it low?
            mode_low=1;         // Set low flag
            mode_high=0;        // Clear high flag
        }
        else{                   // No, it's high
            mode_low=0;         // Clear low flag
            mode_high=1;        // Set high flag    
        }
        mode_last=MODE;         // Update last state
    //    delay_ms(50);           // Debounce
    }
    
}

void switch_init(){                     // Get power on state
    mode_last=MODE;                     // Update last state
}