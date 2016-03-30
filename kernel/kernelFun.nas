; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; オブジェクトファイルを恬るモ�`ド	
[INSTRSET "i486p"]				; 486の凋綜まで聞いたいという��峰
[BITS 32]						; 32ビットモ�`ド喘の�C亠�Zを恬らせる
[FILE "naskfunc.nas"]			; ソ�`スファイル兆秤��

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
