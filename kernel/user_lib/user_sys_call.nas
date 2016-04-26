[FORMAT "WCOFF"]				; 
[INSTRSET "i486p"]				;
[BITS 32]						;
[FILE "sys.nas"]			    ;	
		GLOBAL	_io_in8,  _io_in16,  _io_in32
		GLOBAL	_io_out8, _io_out16, _io_out32
		
		GLOBAL	_send
		GLOBAL 	_receive
		GLOBAL  _sleep
		GLOBAL  _awake
		GLOBAL  _draw
		GLOBAL  _transTolinerAddr
		GLOBAL  _get_pid
		GLOBAL  _assertion_failure
		GLOBAL  _drawNum
[SECTION .text]
_drawNum:
		PUSH	EBP
		MOV		EBP,ESP	
		
		PUSH	EBX
		PUSH	ECX
		PUSH	EDX
		PUSH	ESI
		;系统调用参数
		MOV		EAX,32
		MOV		EBX,[EBP+8]
		MOV		ECX,[EBP+12]
		MOV		EDX,[EBP+16]
		INT 	0X88
		;EAX中有返回值
		POP		ESI
		POP		EDX
		POP		ECX
		POP		EBX
		
		POP		EBP
		RET
_assertion_failure:
		PUSH	EBP
		MOV		EBP,ESP	
		
		PUSH	EBX
		PUSH	ECX
		PUSH	EDX
		PUSH	ESI
		;系统调用参数
		MOV		EAX,28
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
_get_pid:
		PUSH	EBP
		MOV		EBP,ESP	
		
		PUSH	EBX
		PUSH	ECX
		PUSH	EDX
		PUSH	ESI
		;系统调用参数
		MOV		EAX,24
		INT		0X88
		
		;EAX中有返回值
		POP		ESI
		POP		EDX
		POP		ECX
		POP		EBX
		
		POP		EBP
		RET
_transTolinerAddr:
        PUSH	EBP
		MOV		EBP,ESP	
		
		PUSH	EBX
		PUSH	ECX
		PUSH	EDX
		PUSH	ESI
		;系统调用参数
		MOV		EAX,20
		MOV		EBX,[EBP+8]
		MOV		ECX,[EBP+12]
		MOV		EDX,[EBP+16]
		INT		0X88
		;EAX中有返回值
		POP		ESI
		POP		EDX
		POP		ECX
		POP		EBX
		
		POP		EBP
		RET
;u_int32 draw(u_int8 *str ,u_int32 key,u_int32 pid                                             );	
_draw:
		PUSH	EBP
		MOV		EBP,ESP	
		
		PUSH	EBX
		PUSH	ECX
		PUSH	EDX
		PUSH	ESI
		;系统调用参数
		MOV		EAX,16
		MOV		EBX,[EBP+8]
		MOV		ECX,[EBP+12]
		MOV		EDX,[EBP+16]
		INT		0X88
		;EAX中有返回值
		POP		ESI
		POP		EDX
		POP		ECX
		POP		EBX
		
		POP		EBP
		RET
;u_int32 send(struct MESSAGE*msg,u_int32 call_pid                                              );
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