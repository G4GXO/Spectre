; "SPECTRE" Speech Inverter
;
; (c)Ron Taylor G4GXO
;  
; Audio Processing Block
;
; Register used
; w0,w1,w4,w5,w6,w8,w9,w10,
;
;*********************************************************************************************
   
    .text  
	.global _af_processor		; Make the program visible to the C routines

_af_processor:
; Save critical registers
    push    MODCON                          ; save context of MODCON
    push    CORCON                          ; save context of CORCON
    push    w0
    push    w1
    push    w4
    push    w5
    push    w6
    push    w8
    push    w9
    push    w10

;*********************************************************************************************
;
; Prepare DSP Core and Modulo Addressing for filter delay lines and coefficients
;
;*********************************************************************************************
;
; Set core mode for DSP calculations
;
       mov     #0x00F0,w8     		; Enable accumulator A and B Super Saturation and Data Space write Saturation,
       mov     w8, CORCON               ; signed fractional multiplies

       mov     #0x8FF8, w8              ; Enable modulo addressing in X-Memory, bit reversing disabled,
       mov     w8, MODCON               ; w8 used for modulo addressing, Y modulo addressing disabled
       nop				; nop added for safety, see Data Memory section 3.2
       
       ;bra carrier_gen
;
; Apply Mic Gain Factor for Tx
	
	mov _mic_in,w4
	mov _mic_gain_factor,w5
	mpy w4*w5,A
	sac.r A, w0
	mov w0, _mic_audio
	
;*********************************************************************************************
;
; Input FIR Filter
;
; Universal FIR Filter
;
;*********************************************************************************************	
input_filter:
	mov _fir_in, w0				; Get input and load into w0
	mov #_fir_in_delay,w8			; Load delay line start address into w8
	mov w8, XMODSRT				; XMODSRT = start address of delay line
	mov _fir_in_delay_end, w8		; Load delay line end address
	mov w8, XMODEND				; XMODEND = end address of delay line
	    mov _fir_m, w1			; Load number of taps-1 (M) into w1
	    mov _fir_in_delay_ptr, w8		; Load delay line pointer into w8
	    mov w0, [w8++]			; Move new sample into position pointed to by w8
	    mov w8, _fir_in_delay_ptr		; Update delay line pointer
	    mov #_fir_coeff, w10		; Point to start of coefficients
	    clr A, [w8]+=2, w5, [w10]+=2, w6	; Clear MAC Accumulator A, load w5 with sample pointed to by w8 and w6 with that at w10
	    repeat w1				; Repeat MAC instruction w1 times = Number of taps - 1
	    mac w5*w6, A, [w8]+=2, w5, [w10]+=2, w6
	    sac.r a,w0
	    mov	w0,_AF_norm_out			; Save output to filtered non inverted output
	    
;*********************************************************************************************
; Carrier Oscillator
;
; Calculate AF oscillator signal for CW generation and sidetone
;
; Maclaurin Series sine approximation
;
; sin(x)=a*x + b*x^2 + c*x^3 + d*x^4 + e*x^5
;
; Phase input in 1.15, coefficients scales to 4.12, output in 1.15. Calculation operates
; on first quadrant and is reflected and mirrored into the upper three quadrants determined
; by the states of the upper two bits of the phase word.
;
; As memory is not constrained in this application, the sine generator routine is duplicated
; to save several instruction through calls to a common routine.
;
; Input 	- Phase word
; Output 	- Sine value in w0
;
;*********************************************************************************************
carrier_gen:
; Add in phase increment to phase accumulator
	mov _carrier_phase_inc,w0		; Copy phase increment to w0
	mov _carrier_phase_word,w1		; Copy current phase accumulator word into w1
	add w0,w1,w1				; Add phase increment to phase accumulator
	mov w1, _carrier_phase_word		; Save to phase accumulator
; Modify phase so that it can be computed as a first quadrant value	
	btsc _carrier_phase_word, #14       	; Test phase quadrant bit
	neg	w1, w1				; In quadrants 2 or 3, complement phase
	mov	#0x7FFF, w0			; Mask off sign bit
	and	w0, w1, w4			; Save to w4
; Compute sine polynomial
 	mov	#_sin_coeff,w9			; Point to start of sine coeff table
	clr	A, [w9]+=2, w5			; Clear ACCA and load first sine coeff into w5, post increment coeff address
	mov	w4, w6				; Copy phase word to w6 for power raising
	mac	w4*w5, A, [w9]+=2, w5		; First multiply of phase (w4) and coefficient (w5), prefetch next sine coeff to w5
	do	#3, sin_end                    ; Loop 4 times to calculate remaining polynomial terms
		mpy w4*w6, B			; Raise power of phase factor
		sac.r	B, w6			; Store rounded result to w6 for next polynomial
sin_end:	mac	w5*w6, A, [w9]+=2, w5	; Multiply raised phase (w6) and coefficient (w5), prefetch next sine coeff to w5
	sac.r	A, #-3,w4			; Multiply by 8 to account for coefficient scaling and store rounded ACCA to w4
	btsc    w4,#15				; Output here should be positive, if rounding errors taken the result past 0x7fff 
	mov #0x7fff, w4				; (negative in Q1.15), then hold at 0x7fff    
	btsc _carrier_phase_word, #15		; Test the msb of the current phase word, if it is set then
	neg w4, w4      			; phase >180 degrees and output is negative, save to w4
	mov w4,w0				; Copy to w0 for mute operation					
	mov w0,_carrier_out_rx			; Copy to carrier out tx (without mute) for frequency monitor/measurement
; Set audio mute
	btsc _AF_mute,#0			; Is mute ON (1)?	    
	clr w0					; Yes, clear w4
	mov w0,_carrier_out_tx			; Copy to carrier out tx (with mute) for frequency monitor/measurement
	
;*********************************************************************************************
;	    
; Mixer	    
;
;*********************************************************************************************
mixer:	
	mov _AF_norm_out,w5			; Copy filtered input to w5
	mpy w4*w5,A				; Multiple by carrier    
	sac.r A,w0				; Round and save DSBSC to w0

;*********************************************************************************************
;
; Output FIR Filter
;
; Universal FIR Filter
;
;*********************************************************************************************	    

op_filter:	
	mov #_fir_out_delay,w8			; Load delay line start address into w8
	mov w8, XMODSRT				; XMODSRT = start address of delay line
	mov _fir_out_delay_end, w8		; Load delay line end address
	mov w8, XMODEND				; XMODEND = end address of delay line
	    mov _fir_m, w1			; Load number of taps (N) into w1
	    mov _fir_out_delay_ptr, w8		; Load delay line pointer into w8
	    mov w0, [w8++]			; Move new sample into position pointed to by w8
	    mov w8, _fir_out_delay_ptr		; Update delay line pointer
	    mov #_fir_coeff, w10		; Point to start of coefficients
	    clr A, [w8]+=2, w5, [w10]+=2, w6	; Clear MAC Accumulator A, load w5 with sample pointed to by w8 and w6 with that at w10
	    repeat w1				; Repeat MAC instruction w1 times = Number of taps - 1
	    mac w5*w6, A, [w8]+=2, w5, [w10]+=2, w6
	    sac.r a,#-1,w0			; Round, apply 6dB gain for conversion loss and save to w0
af_exit:
	    mov w0,_AF_inv_out			; Save to inverted AF output 

;*********************************************************************************************
;
; Sidetone
;
; Transmit sidetone for voice
;
;*********************************************************************************************	    	    
	    clr w4				; Clear w4
	    btss _AF_mute,#0			; Is mute ON (1)?	    
	    mov _mic_audio,w4			; Yes, copy current mic sample to w4
	    asr w4,#3,w4			; Drop the level of w4
	    mov w4, _sidetone_audio		; Copy w4 to sidetone audio
	    
;*********************************************************************************************
;
; Control Tone Generator
;
;
;*********************************************************************************************		    
	    
; Modify phase so that it can be computed as a first quadrant value
    mov _tone_phase_inc,w0			; Copy tone phase increment into w0
    mov _tone_phase_word,w1          		; Copy tone phase word to w1
    add w0,w1,w1                                ; Add in phase increment
    mov w1, _tone_phase_word                    ; Save back to tone phase accumulator
	btsc _tone_phase_word, #14       	; Test phase quadrant bit
	neg	w1, w1				; In quadrants 2 or 3, complement phase
	mov	#0x7FFF, w0			; Mask off sign bit
	and	w0, w1, w4			; Save to w4
; Compute sine polynomial
	mov	#_sin_coeff,w9			; Point to start of sine coeff table
	clr	A, [w9]+=2, w5			; Clear ACCA and load first sine coeff into w5, post increment coeff address
	mov	w4, w6				; Copy phase word to w6 for power raising
	mac	w4*w5, A, [w9]+=2, w5		; First multiply of phase (w4) and coefficient (w5), prefetch next sine coeff to w5
	do	#3, tone_end                    ; Loop 4 times to calculate remaining polynomial terms
		mpy w4*w6, B			; Raise power of phase factor
		sac.r	B, w6			; Store rounded result to w6 for next polynomial
tone_end:	mac	w5*w6, A, [w9]+=2, w5	; Multiply raised phase (w6) and coefficient (w5), prefetch next sine coeff to w5
	sac.r	A, #-3,w4			; Multiply by 8 to account for coefficient scaling and store rounded ACCA to w0
	btsc    w4,#15				; Output here should be positive, if rounding errors taken the result past 0x7fff 
	mov #0x7fff, w4				; (negative in Q1.15), then hold at 0x7fff    
	btsc _tone_phase_word, #15		; Test the msb of the current phase word, if it is set then
	neg	w4, w4      			; phase >180 degrees and output is negative, save to w0
; Set output level
; Apply fractional gain factor to output sample
    mov _tone_shaping,w5                        ; Load output gain into w1
    mpy w4*w5,A                                 ; DSP fractional multiply to incorporate left shift
    sac.r A,w0                                  ; Round and store to w0
    asr w0,#3,w0
    mov w0, _tone_out                           ; Copy to output	    
;
;*********************************************************************************************
;
    Exit:  
    
    pop    w10
    pop	   w9
    pop    w8   
    pop	   w6
    pop    w5    
    pop    w4  
    pop    w1    
    pop    w0   
    
    pop     CORCON                          ; restore context of CORCON
    pop     MODCON                          ; restore context of MODCON
    nop

    return
    
    .end


