; naskfunc
; TAB=4

[FORMAT "WCOFF"]				; ���֥������ȥե�����������`��	
[INSTRSET "i486p"]				; 486������ޤ�ʹ�������Ȥ���ӛ��
[BITS 32]						; 32�ӥåȥ�`���äΙCе�Z�����餻��
[FILE "naskfunc.nas"]			; ���`���ե����������

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
