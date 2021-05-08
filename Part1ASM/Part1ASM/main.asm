;
; Part1ASM.asm
;
; Author : kamph
;

; Replace with your application code
start:
    ; initialize stack pointer
    ldi r16,HIGH(RAMEND)
	out SPH,r16
	ldi r16,LOW(RAMEND)
	out SPL,r16
	RCALL NeoPixel_Init
	RCALL AD_Init
	LDI R24, 25
	RCALL delay
	nop
while:
    RCALL Clear_Pixels
    RCALL Read_Light
	;LDI R24,100
	; The code below calculates how many neopixels to turn on based off R24 (originally the light sensor)
	;  Basically finds the percentage (R24/64) and looks at only the tens digit 
	MOV R17,R24 ; Copy R24
	ANDI R17,0x3F ; Limit R24 to be 0-63
	LDI R16,39
	MUL R17,R16 ; Values: 0x0000-0x0999 OR 0-2457
	LDI R16,9
	CLC
	; Find the 9 complement of the high byte (9-X=COMPLEMENT)
	SBC R16,R1 ; R16 = 9 - high_byte - 0 (0x00-0x09)
	LDI R17,3
	; Multiply by 3 (1 for R, 1 for G, 1 for B)
	MUL R17,R16 ; R17 = 3 * R16
	MOV R16,R0 ; Use only low byte (max value of 0x1B=27)
	; Load array into X
	LDI R26, low(NeoPixel_Array)
	LDI R27, high(NeoPixel_Array)
	LDI R24, 0x10 ; R24 = 16
For_Loop:
	ST X+,R24 ; Store 16 into array for indexes 0-R24
	DEC R16
	BRNE For_Loop 
	; Display Temperature as color of NeoPixel #9 
	LDI R26, low(NeoPixel_Array+27)
	LDI R27, high(NeoPixel_Array+27)
	RCALL Read_Temp
	LSR R24 ; Value is 0 to 127
	MOV R17,R24 ; Copy R24, originally the read temp
	; Debug force value
	;LDI R17,90
	LDI R16,32
	CP R17,R16
	BRMI all_blue ; Branch if temp < 32
	LDI R16,96
	CP R17,R16
	BRPL all_red ; branch if temp > 96
	LDI R16,64
	CP R17,R16          ; (value*2)-64
	BRMI LT_64 ; Branch if temp < 64
	// GT_64
	LDI R16,64
	CLC 
	SBC R17,R16         ; R17 = (Rx)-64
	LSL R17             ; R17 = (Rx*2)-128
	ST X+,R17           ; Red = (value*2)-128
	MOV R17,R24
	NEG R17             ; R17 = (-Gx)
	LDI R16,96
	ADD R17,R16         ; R17 = -Gx + 96
	LSL R17             ; R17 = -2*Gx + 192
	ST X+,R17           ; Green = (-2*value) + 192
	LDI R16,0
	ST X+,R16           ; Blue = 0
	rjmp exit_if
LT_64:
    LDI R16,0
	ST X+,R16           ; Red = 0
	MOV R17,R24
	LDI R16,32
	CLC 
	SBC R17,R16         ; R17 = (Gx)-32
	LSL R17             ; R17 = (Gx*2)-64
	ST X+,R17           ; Green = (value*2)-64
	MOV R17,R24
	NEG R17             ; R17 = (-Bx)
	LDI R16,64
	ADD R17,R16         ; R17 = -Bx + 64
	LSL R17             ; R17 = -2*Bx + 128
	ST X+,R17           ; Blue = (-2*value) + 128
	rjmp exit_if
all_blue:
    LDI R16,0
	ST X+,R16           ; Red = 0
	ST X+,R16           ; Green = 0
	LDI R16,64
	ST X+,R16           ; Blue = 64
	rjmp exit_if
all_red:
    LDI R16,64
	ST X+,R16           ; Red = 64
	LDI R16,0
	ST X+,R16           ; Green = 0
	ST X+,R16           ; Blue = 0
exit_if:


	RCALL Update_Pixels

	LDI R24, 25
	RCALL delay

stop:

	rjmp while

    rjmp start
	
;**** This code is not being used ****	
    ; NeoPixel 1 RED
	LDI R24, 0x3f
	LDI R22, 0
	LDI R20, 0
	RCALL Send_Pixel
;	LDI R24, 25
;	RCALL delay
	; NeoPixel 1 Green
	LDI R24, 0
	LDI R22, 0x3f
	LDI R20, 0
	RCALL Send_Pixel
;	LDI R24, 25
;	RCALL delay

	; NeoPixel 1 Blue
	LDI R24, 0
	LDI R22, 0
	LDI R20, 0x3f
	RCALL Send_Pixel
	LDI R24, 50
	RCALL delay

	rjmp while

    rjmp start
	              ; R0    G0    B0,   R1    G1    B1,   R2    G2    B2,   R3    G3    B3,   R4    G4    B4,   R5    G5    B5,   R6    G6    B6,   R7    G7    B7,   R8    G8    B8,   R9    G9    B9
NP_Init_values: .DB 0x30, 0x00, 0x00, 0x30, 0x10, 0x00, 0x20, 0x20, 0x00, 0x10, 0x30, 0x00, 0x00, 0x30, 0x10, 0x00, 0x20, 0x20, 0x00, 0x10, 0x30, 0x00, 0x00, 0x30, 0x10, 0x00, 0x20, 0x20, 0x00, 0x10

NeoPixel_Init:
	; Store the registers in the stack
    PUSH R31
	PUSH R30
	PUSH R27
	PUSH R26
	PUSH R24
	PUSH R16
	; set PB0 to an output, initial value is '0'
	CBI 5,0 
	SBI 4,0
	; Load the array memory address into X
	LDI R26, low(NeoPixel_Array)
	LDI R27, high(NeoPixel_Array)
	; Load the init values into Z
	LDI R30, low(NP_Init_values<<1)
	LDI R31, high(NP_Init_values<<1)
	LDI R24, 30
NP_For_Loop:
	; Load a byte into R16 then increment Z
	LPM R16,Z+
	; Store R16 into the byte array
	ST X+,R16
	DEC R24
	BRNE NP_For_Loop ; Loop 30 times
	RCALL Update_Pixels
	; Restore registers from stack
	POP R16
	POP R24
	POP R26
	POP R27
	POP R30
	POP R31
	RET

Clear_Pixels:
	; Registers onto stack
    PUSH R27
	PUSH R26
    PUSH R24
	PUSH R16
	; Load array into X
	LDI R26, low(NeoPixel_Array)
	LDI R27, high(NeoPixel_Array)
	LDI R24, 30
	LDI R16,0
CP_For_Loop:
	; Store 0 into each array index
	ST X+,R1
	DEC R24
	BRNE CP_For_Loop 
	; Restore registers from stack
	POP R16
	POP R24
	POP R26
	POP R27
	RET	

Update_Pixels:
	; Store registers into stack
    PUSH R27
	PUSH R26
    PUSH R24
	PUSH R22
	PUSH R20
	PUSH R16
	LDI R16,10
	; Load NeoPixel_Array address into X
	LDI R26, low(NeoPixel_Array)
	LDI R27, high(NeoPixel_Array)
UP_For_Loop:
	; Load the RGB values into registers 20, 22, and 24
	LD R24,X+
	LD R22,X+
	LD R20,X+
	RCALL Send_Pixel
	DEC R16
	BRNE UP_For_Loop ; Loop 10 times, 1 per neopixel
	; Restore registers from stack
	POP R16
	POP R20
	POP R22
	POP R24
	POP R26
	POP R27
	RET


Send_Pixel:
	; Three parameters: R = R24, G = R22 and B = R20
	; G needs to be sent first, then R and the B
	; Registers onto stack
	PUSH R24
	PUSH R23
	PUSH R22
	PUSH R21
	PUSH R20
	RCALL byte_loop
	MOV R22, R24 ; Get the next value
	RCALL byte_loop
	MOV R22, R20 ; Get the next value
	RCALL byte_loop
	; Restore registers from stack
	POP R20
	POP R21
	POP R22
	POP R23
	POP R24
	RET
byte_loop:
	LDI R23,8
	;set PB0 to one
next_bit:
	SBI 5,0
	; Put the first bit into C
	ROL R22
	; Branch if bit set
	BRCS send_one
; send_zero:
;	LDI R21,2           ; 7 clock cycles high
;wait_zero_high:
;	DEC R21
;	BRNE wait_zero_high
	; clear PB0 to zero
	CBI 5,0
	LDI R21,1          ; 14 clock cycles low
wait_zero_low:
	DEC R21
	BRNE wait_zero_low
	RJMP exit_zero
send_one:
	LDI R21,2           ; 14 clock cycles high
wait_one_high:
	DEC R21
	BRNE wait_one_high
	; clear PB0 to zero
	CBI 5,0
;	LDI R21,2          ; 7 clock cycles low
;wait_one_low:
;	DEC R21
;	BRNE wait_one_low
exit_zero:
	DEC R23
	BRNE next_bit ; Loop until all bits of R22 sent
	RET	

AD_Init:
    PUSH R16
	LDI R16,0x96     ; Enable='1', Start ='0', Auto-Trigger='0', Flag='1' (to clear flag), IE='0' (disabled), Prescale=clock/64
	STS ADCSRA,R16
	LDI R16,0xE0     ; Ref = Internal, Left Justifed data, MUX=channel A0
	STS ADMUX,R16
	LDI R16,0x81
	STS DIDR0,R16 ; Disable the digital input buffer for ADC7 and ADC0, enable for rest
	STS PORTF,R16 ; Output 0x81
	POP R16
	RET

Read_Light:
	; R16 onto stack
    PUSH R16
	; Set analog input to ADC0
	LDS R16,ADMUX
	ANDI R16,0xE0
	STS ADMUX,R16
	; Set interrupt flag and start conversion
	LDS R16,ADCSRA
	ORI R16,0x50
	STS ADCSRA,R16
RL_while:
	LDS R16,ADCSRA
	ANDI R16,(1<<4)
	BREQ RL_while ; Loop until bit 4 is cleared (ADc Interrupt Flag)
	LDS R24,ADCH ; Store high byte of ADc into R24
	; Restore R16 from stack
	POP R16
	RET

Read_Temp:
	; R16 onto stack
    PUSH R16
	; Set analog input to ADC6
	LDS R16,ADMUX
	ANDI R16,0xE0
	ORI R16,0x07
	STS ADMUX,R16
	; Set interrupt flag and start conversion
	LDS R16,ADCSRA
	ORI R16,0x50
	STS ADCSRA,R16
RT_while_1:
	LDS R16,ADCSRA
	ANDI R16,(1<<4)
	BREQ RT_while_1 ; Loop until bit 4 is clearded (ADc Intterrupt Flag)
	LDS R24,ADCH ; Store high byte of ADc into R24
	POP R16
	RET

delay:
	; Store registers into stack
   PUSH R24
   PUSH R18
   PUSH R17
d1:
    LDI R17,0xFF
d2:
	LDI R18,0xFF
d3:
	DEC R18
	BRNE d3 ; Loop 255 times
	DEC R17
	BRNE d2 ; Loop 255 times
	DEC R24
	BRNE d1 ; Loop 256 times
	; Restore registers from stack
	POP R17
	POP R18
	POP R24
    ret

.dseg
NeoPixel_Array: .byte 30   ; reserves 30 bytes for neopixels (RGB to 10 pixels)