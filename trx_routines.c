// "SPECTRE" Speech Inverter
//
// (c)Ron Taylor G4GXO
//
// PTT Function

int ptt_up;             // PTT up flag
int ptt_down;           // PTT down flag
int ptt_new;            // Current PTT state
int ptt_last;           // Previous PTT state

void ptt_test(){
    
    ptt_new=PTT;                // Get current PTT state
    if(ptt_new!=ptt_last){      // Has it changed?
        if(!PTT){               // Yes, find out which way and set flags accordingly
            ptt_down=1;         // Mark change to Tx
            ptt_up=0;
        }
        else if(PTT){           // Mark change to Rx
            ptt_down=0;
            ptt_up=1;
        }
        delay_ms(100);          // Debounce
    }
    ptt_last=ptt_new;           // Update historic record
}
