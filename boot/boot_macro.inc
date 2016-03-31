;/************************************************************************************
;Thu Mar 31 15:02:03 2016
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
;set segment descriptor(base:limit:attribute)
%macro Descriptor 3
		dw %2 & 0FFFFh
		dw %1 & 0FFFFh
		db (%1>>16) & 0FFh
		dw ((%2>>8) & 0f00h) | (%3 & 0f0ffh)
		db (%1>>24) & 0ffh
%endmacro
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;set Cylinder,head,sector and AL=1(number of sector)
%macro setCHS 4                     
	   MOV 	CH,(%1 & 0X00FF)
	   MOV	CL,(%3) | ((%1&0X0300)>>2)
	   MOV	DH,%2
	   MOV	DL,0
	   MOV	AL,%4
%endmacro
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;set readBuff,ES:BX=(%1,%2)
%macro setReadBuff 2                
	   MOV	AX,%1
	   MOV	ES,AX
	   MOV	BX,%2
%endmacro
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
%macro setMcpySrcDescNum 3
	   MOV	ESI,%1
	   MOV	EDI,%2
	   MOV	ECX,%3
%endmacro
;100=1100100
;MOV	CH,100D
;MOV	CL,0
;MOV	DH,0
;MOV	DL,0
;MOV	AL,1