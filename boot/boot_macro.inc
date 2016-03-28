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
	   MOV	CL,(%3) | (%1 & (0X0300>>2))
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