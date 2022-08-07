	; ============== register usage =================|
    ; ------ RB0: ---------------------------------- |
    ; R0 -> keyboard scanning index                  |
    ; R1 -> row counter for scanning keyboard        |
    ; R2 -> keyboard scanning output                 |
    ; R5, R6, R7 -> delay counter                    |
    ; ------ RB1: ---------------------------------- |
    ; R0, R1, R2 -> seven segment current value      |
    ; R3 -> count down delay counter                 |
    ; R4 -> which calculation
	; R5, R6, R7
    ; R7 -> delay counter                            |
    ; ===============================================|

				ORG 0000H
				SJMP MAIN
				ORG 03H
				JMP COUNT_DOWN
				ORG 13H
				JMP EQUAL
				
				ORG 0030H	
MAIN:			MOV IE, #10000101B
				MOV SP, #32H
				SETB IT0				; falling EDGE-trigger
				SETB IT0
				
				SETB RS0			; switch to RS1
				MOV R0, #0  		; R0 ~ R6 in RB1 set to 0
				MOV R1, #0
				MOV R2, #0
				MOV R4, #0
				MOV R5, #0
				MOV R6, #0
				CLR RS0     		; switch back to RB0
				
START_SCAN:		MOV R0, #0			; 2 LED
				MOV R1, #4			; set 4 rows
				MOV R2, #11110111B	; keyboard scanning output

SCAN:			MOV A, R2
				MOV P2, A			; ouyput keyboard scanning
				JNB P2.7, KEYIN
				INC R0
				JNB P2.6, KEYIN
				INC R0
				JNB P2.5, KEYIN
				INC R0
				JNB P2.4, KEYIN
				INC R0
				RR A
				MOV R2, A
				DJNZ R1, SCAN
KEY_OUT:		CALL SHOW_SEG				
				JMP START_SCAN
				
KEYIN:			CALL DEBOUNCE
				MOV DPTR, #KEY_TABLE; put offset into A
				MOV A, R0
				MOVC A, @A+DPTR
				
				CJNE A, #0CH, NEXT1 ; if press C, go to MULTIPLE
				JMP MULTIPLE
NEXT1:			CJNE A, #0AH, NEXT2	; if press A, go to PLUS
				JMP PLUS
NEXT2:			CJNE A, #0BH, NEXT3	; if press B, go to MINUS
				JMP MINUS
NEXT3:			CJNE A, #0DH, NEXT4	; if press D, go to DIVIDE
				JMP DIVIDE
NEXT4:			CJNE A, #0EH, NEXT5	; if press E, go to DIVIDE
				JMP DIVIDE
NEXT5:			CJNE A, #0FH, NEXT6	; if press F, go to DIVIDE
				JMP DIVIDE
NEXT6:			CALL SAVE_KEY_IN
				JMP KEY_OUT
				
COUNT_DOWN:		PUSH PSW
				SETB RS0
				
DEC_LOOP:		MOV R3, #100
SHOW_LOOP:		CALL SHOW_SEG
				DJNZ R3, SHOW_LOOP
				MOV A, R2
				JZ DEC_R1
				DEC R2
				JMP DEC_LOOP
DEC_R1:			MOV A, R1
				JZ DEC_R0			;if R1 == 0, go to DEC_R0
				DEC R1
				MOV R2, #9
				JMP DEC_LOOP
DEC_R0:			MOV A, R0
				JZ ALL_ZERO			;if R1 == 0, go to ALL_ZERO
				DEC R0
				MOV R1, #9
				MOV R2, #9
				JMP DEC_LOOP
ALL_ZERO:		POP PSW
				RETI
				
INIT_CAL:		MOV A, R2			; save R2 -> R5
				MOV R5, A
				MOV A, R1
				MOV R6, A
				MOV A, R0
				MOV R7, A
				MOV R2, #0
				MOV R1, #0
				MOV R0, #0
				RET
				
PLUS:			PUSH PSW
				SETB RS0
				MOV R4, #1
				CALL INIT_CAL	
				JMP POP_OUT	
				
MINUS:			PUSH PSW
				SETB RS0
				MOV R4, #2
				CALL INIT_CAL
				JMP POP_OUT

MULTIPLE:		PUSH PSW
				SETB RS0
				MOV R4, #3			; seting R4 to 2 means MINUS
				CALL INIT_CAL
				JMP POP_OUT

DIVIDE:			PUSH PSW
				SETB RS0
				MOV R4, #4			; seting R4 to 2 means MINUS
				CALL INIT_CAL
				JMP POP_OUT
				
POP_OUT:		POP PSW
				JMP KEY_OUT

EQUAL:			PUSH PSW
				SETB RS0
				CJNE R4, #1, EQ_MINUS
				MOV A, #10
				MOV B, R6
				MUL AB
				MOV B, R5
				ADD A, B
				MOV R7, A
				
				MOV A, #10
				MOV B, R1
				MUL AB
				MOV B, R2
				ADD A, B
				
				MOV B, A
				MOV B, R7
				ADD A, B
				
				MOV B, #10
				DIV AB
				MOV R1, A
				MOV R2, B
				CALL SHOW_SEG
				POP PSW
				RETI
EQ_MINUS:		CJNE R4, #2, EQ_MUL
				MOV A, R5
				MOV B, R2
				SUBB A, B
				MOV R2, A
				CALL SHOW_SEG
				POP PSW
				RETI
EQ_MUL:			CJNE R4, #3, EQ_DIV
				MOV A, R5
				MOV B, R2
				MUL AB
				MOV R2, A
				CALL SHOW_SEG
				POP PSW
				RETI
EQ_DIV:			MOV A, R5
				MOV B, R2
				DIV AB
				MOV R2, A
				CALL SHOW_SEG
				POP PSW
				RETI

SAVE_KEY_IN:	PUSH PSW	; A now keyin put into R1
				SETB RS0	; past R1 put into R0
				MOV R7, A
				MOV A, R1
				MOV R0, A
				MOV A, R2
				MOV R1, A
				MOV A, R7
				MOV R2, A
				POP PSW
				RET
				
DEBOUNCE: 		PUSH PSW
				SETB RS0
				SETB RS1
				MOV R5, #3
DE_1:			MOV R6, #200 		
DE_2:	 		MOV R7, #250 		
				DJNZ R7, $ 			
				DJNZ R6, DE_2 
				DJNZ R5, DE_1
				POP PSW
				RET 				
				
SHOW_SEG:		MOV DPTR, #TABLE
				PUSH PSW
				SETB RS0			; switch to RS1
				
				CLR P0.3
				MOV A, R2
				MOVC A, @A+DPTR
				MOV P1, A
				CALL DEL_330_US
				SETB P0.3
				
				CLR P0.2			; control C2
				MOV A, R1
				MOVC A, @A+DPTR
				MOV P1, A
				CALL DEL_330_US
				SETB P0.2
				
				CLR P0.1			; control C1
				MOV A, R0
				MOVC A, @A+DPTR
				MOV P1, A
				CALL DEL_330_US
				SETB P0.1
				
				POP PSW
				RET
				
DEL_330_US:     MOV R7, #164		; Delay 330 us
				DJNZ R7, $      
				RET 

KEY_TABLE:		DB 01H
				DB 02H
				DB 03H
				DB 0CH
				DB 04H
				DB 05H
				DB 06H
				DB 0DH
				DB 07H
				DB 08H
				DB 09H
				DB 0EH
				DB 0AH
				DB 00H
				DB 0BH
				DB 0FH				

TABLE:			DB 00AH 			; 0
				DB 0FAH				; 1
				DB 04CH				; 2
				DB 068H				; 3
				DB 0B8H				; 4
				DB 029H				; 5
				DB 009H				; 6
				DB 03AH				; 7
				DB 008H				; 8
				DB 028H				; 9
				END