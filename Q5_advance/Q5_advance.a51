			ORG 0000H
			JMP START
			ORG 0030H
START:		MOV A, #11111100B		; initial values
			MOV SP, 30H
			SETB P1.0
			SETB P1.1
			MOV R0, 0				; PUSH counter
			MOV R3, 0				; push-pull judgement
LOOP:		MOV P2, A				; make LED shine in loop
			CALL DELAY
			CJNE R3, #0, PUSHING	; if R3 not 0, jump to PUSHING
			RL A
			JMP LOOP

DELAY:		MOV R5, #4
DELAY1:		MOV R6, #250
DELAY2:		MOV R7, #250
RET_PUSH:	JNB P1.0, INIT_PUSH
RET_POP:	JNB P1.1, INIT_POP
			DJNZ R7, RET_PUSH
			DJNZ R6, DELAY2
			DJNZ R5, DELAY1
			RET

INIT_PUSH:	MOV R3, #1
			SETB P1.0
			JMP RET_PUSH
INIT_POP:	MOV R3, #2
			SETB P1.1
			JMP RET_POP
			
PUSHING:	CJNE R3, #1, POPPING
			PUSH 00A0H
			INC R0
			JMP LOOP
			
POPPING:	POP 00A0H
			DJNZ R0, POPPING
			END
			