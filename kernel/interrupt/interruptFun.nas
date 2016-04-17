;/************************************************************************************
;Wed Apr 13 15:38:40 2016
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
[FILE "interrupt.nas"]			;

		GLOBAL _getErrorCode,_getFrame
		GLOBAL _IRQ0_clock,_IRQ1_keyBoard,_IRQ2_slave,_IRQ3_port2,_IRQ4_port1,_IRQ5_LPT2,_IRQ6_floppyDisk,_IRQ7_LPT1
		GLOBAL _IRQ8_CMOS ,_IRQ9_redirect_IRQ2,_IRQ10_reserved1,_IRQ11_reserved2,_IRQ12_PS2Mouse,_IRQ13_FPU_error,_IRQ14_ATDisk,_IRQ15_reserved3
        EXTERN _IRQ0_clock1,_IRQ1_keyBoard1,_IRQ2_slave1,_IRQ3_port21,_IRQ4_port11,_IRQ5_LPT21,_IRQ6_floppyDisk1,_IRQ7_LPT11
		EXTERN _IRQ8_CMOS1,_IRQ9_redirect_IRQ21,_IRQ10_reserved11,_IRQ11_reserved21,_IRQ12_PS2Mouse1,_IRQ13_FPU_error1,_IRQ14_ATDisk1,_IRQ15_reserved31
[SECTION .text]
_getErrorCode:
		MOV		EAX,[ESP+4]
		RET
_getFrame:
		MOV		EBP,[ESP+4] ;tempFrame
		MOV		EAX,[ESP+12];EDI
		MOV		[EBP],EAX
		MOV		EAX,[ESP+16];DSI
		MOV		[EBP+4],EAX
		MOV		EAX,[ESP+20];EBP
		MOV		[EBP+8],EAX
		MOV		EAX,[ESP+24];ESP
		MOV		[EBP+12],EAX
		MOV		EAX,[ESP+28];EBX
		MOV		[EBP+16],EAX
		MOV		EAX,[ESP+32];EDX
		MOV		[EBP+20],EAX
		MOV		EAX,[ESP+36];ECX
		MOV		[EBP+24],EAX
		MOV		EAX,[ESP+40];EAX
		MOV		[EBP+28],EAX
		MOV		EAX,[ESP+44];EIP
		MOV		[EBP+32],EAX
		MOV		EAX,[ESP+48];CS
		MOV		[EBP+36],EAX
		MOV		EAX,[ESP+52];EFLAGS
		MOV		[EBP+40],EAX
		MOV		EAX,[ESP+56];ESP
		MOV		[EBP+44],EAX
		MOV		EAX,[ESP+60];SS
		MOV		[EBP+48],EAX
		RET
_IRQ0_clock:
		PUSHAD
		CALL _IRQ0_clock1
		POPAD
		IRETD
_IRQ1_keyBoard:
		PUSHAD
		CALL _IRQ1_keyBoard1
		POPAD
		IRETD
_IRQ2_slave:
		PUSHAD
		CALL _IRQ2_slave1
		POPAD
		IRETD
_IRQ3_port2:
		PUSHAD
		CALL _IRQ3_port21
		POPAD
		IRETD
_IRQ4_port1:
		PUSHAD
		CALL _IRQ4_port11
		POPAD
		IRETD
_IRQ5_LPT2:
		PUSHAD
		CALL _IRQ5_LPT2
		POPAD
		IRETD
_IRQ6_floppyDisk:
		PUSHAD
		CALL _IRQ6_floppyDisk1
		POPAD
		IRETD
_IRQ7_LPT1:
		PUSHAD
		CALL _IRQ7_LPT11
		POPAD
		IRETD
;SLAVE
_IRQ8_CMOS:
		PUSHAD
		CALL _IRQ8_CMOS1
		POPAD
		IRETD
_IRQ9_redirect_IRQ2:
		PUSHAD
		CALL _IRQ9_redirect_IRQ21
		POPAD
		IRETD
_IRQ10_reserved1:
		PUSHAD
		CALL _IRQ10_reserved11
		POPAD
		IRETD
_IRQ11_reserved2:
		PUSHAD
		CALL _IRQ11_reserved21
		POPAD
		IRETD
_IRQ12_PS2Mouse:
		PUSHAD
		CALL _IRQ12_PS2Mouse1
		POPAD
		IRETD
_IRQ13_FPU_error:
		PUSHAD
		CALL _IRQ13_FPU_error1
		POPAD
		IRETD
_IRQ14_ATDisk:
		PUSHAD
		CALL _IRQ14_ATDisk1
		POPAD
		IRETD
_IRQ15_reserved3:
		PUSHAD
		CALL _IRQ15_reserved31
		POPAD
		IRETD