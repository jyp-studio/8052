/*
R0: offset index
R1: set row counter
R2: row-judgement
R3: delay
R4: delay

P1: output signal
P2: output row-judgement
*/

			ORG 0 				; start from 0000H
			MOV DPTR, #TABLE 	; DPTR point to TABLE
START: 		MOV R0, #0 			; initialize typed signal
			MOV R1, #3 			; set 3 row
			MOV R2, #11110111B 	; set first row
SCAN: 		MOV A, R2 			; move R2 to A
			MOV P2, A 			; input scanned signal
			JNB P2.7, KEYIN 	; scan column 1
			INC R0 				; add #01 into R0
			JNB P2.6, KEYIN 	; scan column 2
			INC R0 				; add #01 into R0
			JNB P2.5, KEYIN 	; scan column 3
			INC R0 				; add #01 into R0
			RR A 				; next row
			MOV R2, A 			; move A to R2
			DJNZ R1, SCAN 		; scan 3 rows
			SJMP START 			; jump to scan keypad again
KEYIN: 		CALL DEBOUNCE 		; call debounce function
			MOV A, R0 			; get typed signal
			MOVC A, @A+DPTR 	; according A, get signal from TABLE
			MOV P1, A 			; output displayed signal
			SJMP START 			; jump to scan keypad again
DEBOUNCE: 	MOV R4, #50 		; move 50 times into R4
DE_500_us: 	MOV R3, #250 		; move 250 times into R3
			DJNZ R3, $ 			; jump to itself R3 times
			DJNZ R4, DE_500_us 	; delay 25ms
			RET 				; return to main code
TABLE: 		DB 10011111B 		; display 1
			DB 00100101B 		; display 2
			DB 00001101B 		; display 3
			DB 10011001B 		; display 4
			DB 01001001B 		; display 5
			DB 01000001B 		; display 6
			DB 00011011B 		; display 7
			DB 00000001B 		; display 8
			DB 00001001B 		; display 9
			END