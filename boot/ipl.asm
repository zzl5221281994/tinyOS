		ORG		0x7c00
		JMP		entry
entry:
		MOV		AX,0			
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX
		MOV		ES,AX
		
		MOV   	AX,0x07e0       ;read to 0x7e00, two sector
		MOV 	ES,AX
		MOV     BX,0
		MOV 	AX,0202H
		MOV		DX,0
		MOV		CX,0002H        ;
		INT 	13H             ;
        jmp     0x07e00         ;

		times	510d-($-$$) DB 0		

		DB		0x55, 0xaa