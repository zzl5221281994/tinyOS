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
; haribote-ipl
; TAB=4

CYLS	EQU		20				
		ORG		0x7c00			
		JMP		entry

entry:
		MOV		AX,0			
		MOV		SS,AX
		MOV		SP,0x7c00
		MOV		DS,AX

		MOV		AX,0x07e0
		MOV		ES,AX
		MOV		CH,0			
		MOV		DH,0			
		MOV		CL,2			
readloop:
		MOV		SI,0			
retry:
		MOV		AH,0x02			
		MOV		AL,1			
		MOV		BX,0
		MOV		DL,0x00			
		INT		0x13			
		JNC		next			
		ADD		SI,1			
		CMP		SI,5			
		JAE		err			    
		MOV		AH,0x00
		MOV		DL,0x00			
		INT		0x13			
		JMP		retry
next:
		MOV		AX,ES			
		ADD		AX,0x0020
		MOV		ES,AX			
		ADD		CL,1			
		CMP		CL,18			
		JBE		readloop		
		MOV		CL,1
		ADD		DH,1
		CMP		DH,2
		JB		readloop		
		MOV		DH,0
		ADD		CH,1
		CMP		CH,CYLS
		JB		readloop		


		JMP		0x7e00

err:
		MOV		SI,msg
putloop:
		MOV		AL,[SI]
		ADD		SI,1			
		CMP		AL,0
		JE		fin
		MOV		AH,0x0e			
		MOV		BX,15			
		INT		0x10			
		JMP		putloop
fin:
		HLT						
		JMP		fin				
msg:
		DB		0x0a, 0x0a		
		DB		"load error"
		DB		0x0a			
		DB		0

		times	510d-($-$$) DB 0

		DB		0x55, 0xaa
