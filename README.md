# Spectre
Audio Spectrum Reversal module for SSB radio

Many commercial or military HF Single Sideband (SSB) transceivers operate in Upper Sideband (USB) only making them incompatible with Amateur Radio band plans for the lower 
frequency bands where Lower Sideband (LSB) is the convention. Spectre provides a means through which USB or LSB only radios may be made comaptible with opposite sideband operation
without the need to modify the equipment. It does this by reversing the transmit and receive audio spectrums, translating low frequencies to high frequencies and vice versa, such 
that an inverted audio channel may be transmitted or received. This has the effect of "undoing" the audio spectrum reversal when trying to communicate between USB and LSB radios.
Spectre sits between the radio and the microphone and headphone/speaker and provides enough audio output to drive either a hadnset/headset or a small loudspeaker. It monitors the 
Push To Talk (PTT) line switching from transmit to receive along with the host transceiver.

Spectre employs a dsPIC33FJ128GP802 processor. The code is written and compiled with MPLAB-X ASM and XC16 Compiler v1.50. Spectre is the Copyright of RWM Taylor BSc (Hons) C.Eng MIET

Ron Taylor G4GXO


