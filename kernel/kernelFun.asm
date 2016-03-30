; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; 
[INSTRSET "i486p"]				;
[BITS 32]						;
[FILE "naskfunc.nas"]			;

		GLOBAL	_io_hlt
		GLOBAL  _write

[SECTION .text]

_io_hlt:	; void io_hlt(void);
		PUSH	EBP
		MOV		EBP,ESP
		PUSHAD
		HLT
		POPAD
		POP		EBP
		RET
_write:
		PUSH	EBP
		MOV		EBP,ESP
		PUSHAD
		MOV		EDI,[EBP+8]
		MOV		AH, [EBP+12]
		MOV		BYTE[GS:EDI],AH
		POPAD
		POP		EBP
		RET
