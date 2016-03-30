%include "boot\boot_macro.inc"
		ORG		0x7c00
		JMP		entry
entry:
		setReadBuff 0x07e0,0x0000     ;setReadBuff
		setCHS  0d,0,2d,1				  ;setCHS Cylinder,head,sector,sector number
		MOV		AH,2
		INT 	13H
		JC		err
		setReadBuff 0x0800,0x0000
		setCHS  0d,0,3d,1
		MOV		AH,2
		INT 	13H
		JC		err
		JMP     0x7e00
err:
		MOV		AX,CS
		MOV		ES,AX
        mov 	ax,message2
		mov 	bp,ax
		mov 	cx,31d
		mov		ax,01301h
		mov		bx,000ch
		mov		dl,0
		int 10h
		jmp     $
message2: db "read sector error!(C=0,H=0,S=0)"
		times	510d-($-$$) DB 0			
		DB		0x55, 0xaa