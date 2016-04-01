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
