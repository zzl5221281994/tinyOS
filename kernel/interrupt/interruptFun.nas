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
		GLOBAL _getErrorCode
		GLOBAL _get_clock
		GLOBAL _IRQ0_clock,_IRQ1_keyBoard,_IRQ2_slave,_IRQ3_port2,_IRQ4_port1,_IRQ5_LPT2,_IRQ6_floppyDisk,_IRQ7_LPT1
		GLOBAL _IRQ8_CMOS ,_IRQ9_redirect_IRQ2,_IRQ10_reserved1,_IRQ11_reserved2,_IRQ12_PS2Mouse,_IRQ13_FPU_error,_IRQ14_ATDisk,_IRQ15_reserved3
        GLOBAL _sys_call
		
		EXTERN _IRQ0_clock1,_IRQ1_keyBoard1,_IRQ2_slave1,_IRQ3_port21,_IRQ4_port11,_IRQ5_LPT21,_IRQ6_floppyDisk1,_IRQ7_LPT11
		EXTERN _IRQ8_CMOS1,_IRQ9_redirect_IRQ21,_IRQ10_reserved11,_IRQ11_reserved21,_IRQ12_PS2Mouse1,_IRQ13_FPU_error1,_IRQ14_ATDisk1,_IRQ15_reserved31
        EXTERN _kernel_mutex
		EXTERN _process_table,_current_exec_pid
        EXTERN _sys_call_table
[SECTION .text]
_get_clock:
		MOV		EAX,4
		INT 	0X88
		JMP		_get_clock
_getErrorCode:
		MOV		EAX,[ESP+4]
		RET
_IRQ0_clock:
		PUSHAD
		;得到进程表stackFrame的偏移
		MOV		EAX,136
		MUL     DWORD[_current_exec_pid]
		ADD 	EAX,_process_table
		ADD		EAX,16
		;保存现场信息
		MOV		EBX,[ESP+48]
		MOV		[EAX],EBX
		
		MOV		EBX,[ESP+44]
		MOV		[EAX+4],EBX
		
		MOV		EBX,[ESP+40]
		MOV		[EAX+8],EBX
		
		MOV		EBX,[ESP+36]
		MOV		[EAX+12],EBX
		
		MOV		EBX,[ESP+32]
		MOV		[EAX+16],EBX
		
		MOV		EBX,[ESP+28]
		MOV		[EAX+20],EBX
		
		MOV		EBX,[ESP+24]
		MOV		[EAX+24],EBX
		
		MOV		EBX,[ESP+20]
		MOV		[EAX+28],EBX
		
		MOV		EBX,[ESP+16]
		MOV		[EAX+32],EBX
		
		MOV		EBX,[ESP+12]
		MOV		[EAX+36],EBX
		
		MOV		EBX,[ESP+8]
		MOV		[EAX+40],EBX
		
		MOV		EBX,[ESP+4]
		MOV		[EAX+44],EBX
	
		MOV		EBX,[ESP]
		MOV		[EAX+48],EBX
		
		CALL _IRQ0_clock1
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
		
;系统调用	
_sys_call:
		CALL 	[_sys_call_table+EAX]
		IRETD