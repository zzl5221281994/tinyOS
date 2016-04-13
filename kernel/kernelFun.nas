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

		GLOBAL	_io_hlt         ;void io_hlt (                       );
		GLOBAL  _setGdt,_setIdt
		GLOBAL	_io_in8,  _io_in16,  _io_in32
		GLOBAL	_io_out8, _io_out16, _io_out32
		GLOBAL  _load_master_maskWord
		GLOBAL  _load_slave_maskWord
		GLOBAL  _io_delay
		GLOBAL  _hander,_sendEOI_Master,_sendEOI_Slave
		GLOBAL  _open_interrupt
		EXTERN  _drawNum
		EXTERN  _time
[SECTION .text]
_open_interrupt:
		sti
		ret
_sendEOI_Master:
		MOV		AL,0X20
		OUT		20H,AL
		NOP
		NOP
		NOP
		NOP
		RET
_sendEOI_Slave:
		MOV		AL,0X20
		OUT		0XA0,AL
		NOP
		NOP
		NOP
		NOP
		RET
_hander:
		CLI
		PUSH	0X00
		PUSH	0X3C
		PUSH	200
		PUSH	200
		PUSH	DWORD[_time]
		CALL    _drawNum
		ADD		ESP,20
		INC     DWORD[_time]
		CALL 	_sendEOI_Master
		STI
		IRETD
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
_setIdt:
		PUSH	EBP
		MOV		EBP,ESP
		MOV		EAX,[EBP+8]
		CLI
		LIDT	[EAX]
		POP		EBP
		RET
_io_in8:	                    ;   int io_in8(int port);
		MOV		EDX,[ESP+4]		;   port
		MOV		EAX,0
		IN		AL,DX
		RET
_io_in16:	                    ; int io_in16(int port );
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,0
		IN		AX,DX
		RET
_io_in32:	                    ; int io_in32(int port );
		MOV		EDX,[ESP+4]		; port
		IN		EAX,DX
		RET

_io_out8:	                    ; void io_out8(int port, int data );
		MOV		EDX,[ESP+4]		; port
		MOV		AL,[ESP+8]		; data
		OUT		DX,AL
		RET

_io_out16:	                    ; void io_out16(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,AX
		RET

_io_out32:	                    ; void io_out32(int port, int data);
		MOV		EDX,[ESP+4]		; port
		MOV		EAX,[ESP+8]		; data
		OUT		DX,EAX
		RET
_io_delay:
		NOP
		NOP
		NOP
		NOP
		RET
_load_master_maskWord:          ;void load load_master_maskWord(u_int8 word);
		MOV		AL,[ESP+4]
		OUT		0X21,AL
	    RET
_load_slave_maskWord:           ;void load load_slave_maskWord (u_int8 word);
		MOV		AL,[ESP+4]
		OUT		0Xa1,AL
	    RET
		
		
		
		