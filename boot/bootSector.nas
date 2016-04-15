;/************************************************************************************
;Thu Mar 31 15:01:32 2016
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
%include "boot\boot_macro.inc"
		ORG		0x7c00
		JMP		entry
entry:
		setReadBuff 0x0000,0x7e00     	  ;setReadBuff
		setCHS  0d,0,2d,17				  ;setCHS Cylinder,head,sector,sector number
		MOV		AH,2
		INT 	13H
		JC		err
		setReadBuff 0x0a00,0x0000     	  ;setReadBuff
		setCHS  0d,1,1d,18				  ;setCHS Cylinder,head,sector,sector number
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