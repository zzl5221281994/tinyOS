		MOV		AX,0x0700         ;15H中断的入口参数，第一个结构起始地址为0000:7028,0x7000存放bootInfo
		MOV  	ES,AX
		MOV		EDI,32d
        MOV		EBX,0
		MOV		ESI,0             ;计数内存段个数
LOOP1:  MOV		ECX,0020D
		MOV		EDX,0X534D4150
		MOV		EAX,0XE820

		INT 	15H
		JC		FIN
		INC  	ESI
		ADD		EDI,0X14
		CMP		EBX,0
		JE		FIN
		JMP  	LOOP1
	FIN:
	    ;MOV		AX,0X0700
		;MOV		ES,AX
		MOV		DWORD[ES:0],ESI