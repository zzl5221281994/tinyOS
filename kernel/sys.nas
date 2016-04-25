[FORMAT "WCOFF"]				; 
[INSTRSET "i486p"]				;
[BITS 32]						;
[FILE "sys.nas"]			    ;	
		GLOBAL	_send
		GLOBAL 	_receive
		GLOBAL  _sleep
		GLOBAL  _awake
		GLOBAL  _draw
;(struct MESSAGE*msg,u_int32 call_pid                             );
[SECTION .text]
;test
_draw:
		MOV		EAX,16
		MOV		EBX,[ESP+4]
		MOV		ECX,[ESP+8]
		MOV		EDX,[ESP+12]
		INT		0X88
		RET
_send:
        PUSH	EBP
		MOV		EBP,ESP
		
		PUSH	EBX
		PUSH	ECX
		PUSH	EDX
		PUSH	ESI
		;系统调用参数
		MOV		EAX,0        ;sys_call_table+0指向send_msg
		MOV		EBX,[EBP+8]
		MOV		ECX,[EBP+12]
		INT		0x88
		;EAX中有返回值
		POP		ESI
		POP		EDX
		POP		ECX
		POP		EBX
		
		POP		EBP
		RET
; (struct MESSAGE*msg,u_int32 type,u_int32 specify,u_int32 call_pid);
_receive:
		PUSH	EBP
		MOV		EBP,ESP
		
		PUSH	EBX
		PUSH	ECX
		PUSH	EDX
		PUSH	ESI
		;系统调用参数
		MOV		EAX,4       ;sys_call_table+4指向recv_msg
		MOV		EBX,[EBP+8]
		MOV		ECX,[EBP+12]
		MOV		EDX,[EBP+16]
		MOV		ESI,[EBP+20]
		INT		0x88
		;EAX中有返回值
		POP		ESI
		POP		EDX
		POP		ECX
		POP		EBX
		
		POP		EBP
		RET
;extern void sleep  (u_int32 pid,u_int32 status                                         );
_sleep:
		PUSH	EBP
		MOV		EBP,ESP
		
		PUSH	EBX
		PUSH	ECX
		PUSH	EDX
		PUSH	ESI
		;系统调用参数
		MOV		EAX,8
		MOV		EBX,[EBP+8]
		MOV		ECX,[EBP+12]
		INT		0X88
		;EAX中有返回值
		POP		ESI
		POP		EDX
		POP		ECX
		POP		EBX
		
		POP		EBP
		RET
;extern void awake  (u_int32 pid,u_int32 parm                                           );
_awake:
		PUSH	EBP
		MOV		EBP,ESP
		
		PUSH	EBX
		PUSH	ECX
		PUSH	EDX
		PUSH	ESI
		;系统调用参数
		MOV		EAX,12
		MOV		EBX,[EBP+8]
		MOV		ECX,[EBP+12]
		INT		0X88
		;EAX中有返回值
		POP		ESI
		POP		EDX
		POP		ECX
		POP		EBX
		
		POP		EBP
		RET