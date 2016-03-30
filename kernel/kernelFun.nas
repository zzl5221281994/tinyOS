; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; オブジェクトファイルを作るモード	
[INSTRSET "i486p"]				; 486の命令まで使いたいという記述
[BITS 32]						; 32ビットモード用の機械語を作らせる
[FILE "naskfunc.nas"]			; ソースファイル名情報

		GLOBAL	_io_hlt
		GLOBAL  _write
		GLOBAL	_write1

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
