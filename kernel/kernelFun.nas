;/************************************************************************************
;Thu Mar 31 15:04:03 2016
;
;MIT License
;Copyright (c) 2016 zhuzuolang
;
;Permission is hereby granted, free of charge, to any person obtaining a copy
;of this software and associated documentation files (the "Software"), to deal
;in the Software without restriction, including without limitation the rights
;to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;copies of the Software, and to permit persons to whom the Software is
;furnished to do so, subject to the following conditions:
;The above copyright notice and this permission notice shall be included in all
;copies or substantial portions of the Software.
;THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;SOFTWARE.
;************************************************************************************/
[FORMAT "WCOFF"]				; 
[INSTRSET "i486p"]				;
[BITS 32]						;
[FILE "naskfunc.nas"]			;

		GLOBAL	_io_hlt         ;void io_hlt(                  );
		GLOBAL  _setGdt         ;void setGdt(unsigned char*lgdt);
		GLOBAL _callGate_enter  ;void callGate_enter();
		GLOBAL _callGate_return ;void callGate_return();
		GLOBAL _callGate_test
		GLOBAL _callGate_ring3
		GLOBAL _move_to_ring3
		EXTERN _drawNum
[SECTION .text]

_io_hlt:	; void io_hlt(void);
		PUSH	EBP
		MOV		EBP,ESP
		HLT
		POP		EBP
		RET
_setGdt:
		PUSH	EBP
		MOV		EBP,ESP
		MOV		EAX,[EBP+8]
		LGDT	[EAX]
		POP		EBP
		
		POP		EDX
		PUSH	8
		PUSH	EDX
		RETF
		RET
_callGate_enter:
		PUSH	EBP
		MOV		EBP,ESP
		MOV		EDX,[EBP+8]
		PUSH	EDX
		PUSH	0
		CALL    FAR[ESP]
		POP		EAX
		POP		EAX
		POP		EBP
		RET

_callGate_return:
		RETF
_callGate_test:
		PUSH 	0
		PUSH	0x3C
		PUSH	700
		PUSH 	700
		PUSH	1234
		CALL    _drawNum
		ADD		ESP,20
		RETF
_callGate_ring3:
		MOV		EDI,0XE0000000
		LABEL1:
		MOV BYTE[EDI],0X1f
		INC     EDI
		CMP		EDI,0XE0002048
		JNE		LABEL1
		;CALL   DWORD 40+3:0
		JMP		$
_move_to_ring3:
		PUSH	EBP
		MOV		EBP,ESP
		MOV		EAX,[EBP+8]  ;CODE
		MOV		EDX,[EBP+12] ;STACK
		PUSH	EDX
		PUSH	0x1000000
		PUSH	EAX
		PUSH 	0
		RETF
		
		
		