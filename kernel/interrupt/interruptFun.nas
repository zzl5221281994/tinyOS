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
		GLOBAL _IRQ0_clock,_IRQ1_keyBoard,_IRQ2_slave,_IRQ3_port2,_IRQ4_port1,_IRQ5_LPT2,_IRQ6_floppyDisk,_IRQ7_LPT1
		GLOBAL _IRQ8_CMOS ,_IRQ9_redirect_IRQ2,_IRQ10_reserved1,_IRQ11_reserved2,_IRQ12_PS2Mouse,_IRQ13_FPU_error,_IRQ14_ATDisk,_IRQ15_reserved3
        GLOBAL _sys_call
		GLOBAL _save_context
		
		EXTERN _IRQ0_clock1,_IRQ1_keyBoard1,_IRQ2_slave1,_IRQ3_port21,_IRQ4_port11,_IRQ5_LPT21,_IRQ6_floppyDisk1,_IRQ7_LPT11
		EXTERN _IRQ8_CMOS1,_IRQ9_redirect_IRQ21,_IRQ10_reserved11,_IRQ11_reserved21,_IRQ12_PS2Mouse1,_IRQ13_FPU_error1,_IRQ14_ATDisk1,_IRQ15_reserved31
        EXTERN _kernel_mutex
		EXTERN _process_table,_current_exec_pid
		EXTERN _sys_call_table
		EXTERN _global_clock
		EXTERN  _sendEOI_Master
		EXTERN _serverNum
[SECTION .text]
selectorKernelData 	EQU		2*8+1
selectorUserData 	EQU		2*8+4+1
_getErrorCode:
		MOV		EAX,[ESP+4]
		RET
_sys_call:;该调用通过INT指令访问系统调用，由于系统调用里面可能会导致进程阻塞，因此需要保存上下文。
          ;ebx,ecx,edx,esi,分别保存则4个参数，eax保存着调用号
		PUSH	EAX
		MOV		AX,selectorKernelData
		MOV		DS,AX
		POP		EAX
		
		PUSHAD
		CALL	_save_context
		PUSH	ESI
		PUSH	EDX
		PUSH	ECX
		PUSH	EBX
		CALL 	[_sys_call_table+EAX]
		ADD 	ESP,16
		;由于EAX带着返回值，因此不能直接POPAD
		PUSH	EAX
		ADD		ESP,4
		POPAD
		MOV		EAX,[ESP-36];取出压入栈中的返回参数
		
		PUSH	EAX
		MOV		AX,selectorUserData
		MOV		DS,AX
		POP		EAX
		IRETD
_save_context:
;使用规则
;ss
;esp
;eflags
;cs
;eip
;pushad
;call _save_context
		PUSH    EAX
		PUSH	EBX
		PUSH	EDX
		MOV		EAX,164
		MUL     DWORD[_current_exec_pid]
		ADD 	EAX,_process_table
		ADD		EAX,16
		;保存现场信息
		MOV		EBX,[ESP+64]
		MOV		[EAX],EBX
		
		MOV		EBX,[ESP+60]
		MOV		[EAX+4],EBX
		
		MOV		EBX,[ESP+56]
		MOV		[EAX+8],EBX
		
		MOV		EBX,[ESP+52]
		MOV		[EAX+12],EBX
		
		MOV		EBX,[ESP+48]
		MOV		[EAX+16],EBX
		
		MOV		EBX,[ESP+44]
		MOV		[EAX+20],EBX
		
		MOV		EBX,[ESP+40]
		MOV		[EAX+24],EBX
		
		MOV		EBX,[ESP+36]
		MOV		[EAX+28],EBX
		
		MOV		EBX,[ESP+32]
		MOV		[EAX+32],EBX
		
		MOV		EBX,[ESP+28]
		MOV		[EAX+36],EBX
		
		MOV		EBX,[ESP+24]
		MOV		[EAX+40],EBX
		
		MOV		EBX,[ESP+20]
		MOV		[EAX+44],EBX
	
		MOV		EBX,[ESP+16]
		MOV		[EAX+48],EBX
		
		POP		EDX
		POP		EBX
		POP		EAX
		RET
_IRQ0_clock:
		PUSH	EAX
		MOV		AX,selectorKernelData
		MOV		DS,AX
		POP		EAX
		
		CMP		DWORD [_kernel_mutex],0
		JNE		L1
		PUSHAD
		;保存上下文
		CALL    _save_context
		
		CALL _IRQ0_clock1
	L1:
		CALL _sendEOI_Master
		INC		DWORD[_global_clock]
		IRETD
_IRQ1_keyBoard:
		;
		PUSH	EAX
		MOV		AX,selectorKernelData
		MOV		DS,AX
		POP		EAX
		;CMP		DWORD[_kernel_mutex],0
		;JNE     L1_1
		;该段代码在每一个中断前都存在
	;L1_1:
     	INC		DWORD[_kernel_mutex]
		PUSHAD
		STI
		CALL _IRQ1_keyBoard1
		CLI
		POPAD
		DEC		DWORD[_kernel_mutex]
		;注意，由于允许中断重入，因此需要判断中断重入的层数来决定是否需要加载用户DS寄存器
		PUSH	EAX
		CMP		DWORD[_kernel_mutex],0
		JNE     L1_2
		;MOV		EAX,DWORD[_serverNum]
		;CMP		DWORD[_current_exec_pid],EAX
		;JBE	    L1_2
		MOV		AX,selectorUserData
		MOV		DS,AX
		;该段代码在每一个中断前都存在
	L1_2:
		POP		EAX
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
       ;
	    PUSH	EAX
		MOV		AX,selectorKernelData
		MOV		DS,AX
		POP		EAX
		;CMP		DWORD[_kernel_mutex],0
		;JNE     L12_1
		;该段代码在每一个中断前都存在
       ; L12_1:
		INC		DWORD[_kernel_mutex]
		PUSHAD
		STI
		CALL _IRQ12_PS2Mouse1
		CLI
		POPAD
		DEC		DWORD[_kernel_mutex]
		;注意，由于允许中断重入，因此需要判断中断重入的层数来决定是否需要加载用户DS寄存器
		PUSH	EAX
		CMP		DWORD[_kernel_mutex],0
		JNE     L12_2
		;MOV		EAX,DWORD[_serverNum]
		;CMP		DWORD[_current_exec_pid],EAX
		;JBE	    L12_2
		MOV		AX,selectorUserData
		MOV		DS,AX
		;该段代码在每一个中断前都存在
	L12_2:
		POP		EAX
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