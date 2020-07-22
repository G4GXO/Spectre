; "SPECTRE" Speech Inverter
;
; (c)Ron Taylor G4GXO
;
;

;*********************************************************************************************
; ASSEMBLER VARIABLES AND MEMORY ALLOCATION
;
; These definitions allow the assembler files to be integrated within the C program. The global 
; defintions allow variables to be shared by the C and assembler routines. Memory is reserved
; here within the X and Y data space for the filter delay lines and their associated variables.
;
;*********************************************************************************************

; Reserve Y Memory for Filter Kernels and Variables
	.section yfir, bss, ymemory, align(4096)

; Filter Coefficients
	.global	_fir_coeff	    ; Coefficients start
_fir_coeff:	.space 1002	    ; Storage for coefficients in bytes
	.global _fir_coeff_end      ; Coefficients end
_fir_coeff_end: .space 2	    ;
	.global	_fir_n		    ; Kernel length (N)
_fir_n:		.space 2	    ;
	.global _fir_m		    ; M=N-1
_fir_m:		.space 2	    ;	

;
;*********************************************************************************************
; Reserve X Memory for Delay Line and Variables
	.section xfir, bss, xmemory, align(2048)

; Input FIR Delay Line
	.global _fir_in_delay			; Input filter delay line start 
_fir_in_delay:		.space 1002		; Delay line in bytes
; Input FIR Variables
	.global	_fir_in_delay_end		; End of delay line
	.global _fir_in_delay_ptr		; Delay line pointer
_fir_in_delay_end:	.space 	2		; Storage for FIR Variables
_fir_in_delay_ptr:	.space	2
; Output FIR Delay Line
	.global _fir_out_delay			; Output filter delay line start 
_fir_out_delay:		.space 1002		; Delay line in bytes	
; Output FIR Variables
	.global	_fir_out_delay_end		; End of delay line
	.global _fir_out_delay_ptr		; Delay line pointer
_fir_out_delay_end:	.space 	2		; Storage for FIR Variables
_fir_out_delay_ptr:	.space	2
; General FIR Variables	
	.global	_fir_in				; FIR input word
	.global _fir_out			; FIR output word
_fir_in:        .space	2
_fir_out:       .space	2

	.section xosc, bss, xmemory, align(1024)
; Carrier Oscillator and Mixer
    .global _carrier_phase_inc	    ; Carrier phase increment
    .global _carrier_phase_word	    ; Carrier phase accumulator
    .global _carrier_out_rx	    ; Carrier output for Rx frequency measurement
    .global _carrier_out_tx	    ; Carrier output via mute for Tx frequency measurement    
_carrier_phase_inc:	.space 2    ;
_carrier_phase_word:	.space 2    ;
_carrier_out_rx:	.space 2    ;
_carrier_out_tx:	.space 2    ;		
; Tone Generator
    .global _tone_phase_inc     ; Tone phase increment
    .global _tone_phase_word    ; Tone phase accumulator
    .global _tone_gain          ; Output level
    .global _tone_shaping       ; Shaping factor
    .global _tone_out           ; Tone output signal
_tone_phase_inc:    .space 2    ;
_tone_phase_word:   .space 2    ;
_tone_gain:         .space 2    ;
_tone_shaping:      .space 2    ;
_tone_out:          .space 2    ;

;
;*********************************************************************************************
;
; General Variables
    .global _AF_norm_out        ; AF non inverted output
    .global _AF_inv_out		; AF inverted output
    .global _AFs_in             ; AF input subrate
    .global _AFs_out            ; AF output subrate
_AF_norm_out:       .space 2    ;
_AF_inv_out:	    .space 2	;       
_AFs_in:            .space 2	;
_AFs_out:           .space 2    ;
;
;*********************************************************************************************
;
; AF Gain
    .global _AF_gain            ; AF Gain factor
   .global  _AF_mute		; AF mute control    
    .global _mic_gain_factor	; 1dB mic gain factor
    .global _mic_audio		; Adjusted mic audio
    .global _mic_in		; Mic input
    .global _sidetone_audio	; Sidetone audio
    .global _sidetone_factor	; Sidetone gain factor
_AF_gain:	    .space 2    ;
_AF_mute:	    .space 2	;	    
_mic_gain_factor:   .space 2	;
_mic_audio:	    .space 2	;
_mic_in:	    .space 2	;
_sidetone_audio:    .space 2	;
_sidetone_factor:   .space 2	;    
;
;*********************************************************************************************
;

    
