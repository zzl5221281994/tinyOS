[FORMAT "WCOFF"]				; 
[INSTRSET "i486p"]				;
[BITS 32]						;
[FILE "naskfunc.nas"]			;
		
		GLOBAL	_send
		GLOBAL 	_receive
_send:;(struct MESSAGE*msg,u_int32 call_pid                             );
		PUSH	EBP
		MOV		EBP,ESP
		
		PUSH	EBX
		PUSH	ECX
		PUSH	EDX
		PUSH	ESI
		;ϵͳ���ò���
		MOV		EAX,0        ;sys_call_table+0ָ��send_msg
		MOV		EBX,[EBP+8]
		MOV		ECX,[EBP+12]
		INT		0X88
		;EAX���з���ֵ
		POP		ESI
		POP		EDX
		POP		ECX
		POP		EBX
		
		POP		EBP
		RET
_receive:; (struct MESSAGE*msg,u_int32 type,u_int32 specify,u_int32 call_pid);
		PUSH	EBP
		MOV		EBP,ESP
		
		PUSH	EBX
		PUSH	ECX
		PUSH	EDX
		PUSH	ESI
		;ϵͳ���ò���
		MOV		EAX,4       ;sys_call_table+4ָ��recv_msg
		MOV		EBX,[EBP+8]
		MOV		ECX,[EBP+12]
		MOV		EDX,[EBP+16]
		MOV		ESI,[EBP+20]
		INT		0X88
		;EAX���з���ֵ
		POP		ESI
		POP		EDX
		POP		ECX
		POP		EBX
		
		POP		EBP
		RET