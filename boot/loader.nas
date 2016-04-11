;/************************************************************************************
;Thu Mar 31 15:03:42 2016
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
%include "boot\boot_parameter.inc"
%include "boot\boot_macro.inc    "
[bits 16]
org 0x7e00
%include "boot\memory_map.inc   ";此文件包含获取内存分布图的代码，并将得到的内存分布图数据结构保存在0x701c开始的内存里
	JMP		LABEL_BEGIN
;BeginFunctionBlock		各段基地址
;                                       |1MB~~~~~~5MB|  |5MB~~~~~~~~~~~10MB|   
;                                           |代码|       |数据---> <---栈|
KERNEL_SEG_BASE		EQU		0X100000
DATA_SEG_BASE		EQU 	0X000000
topOfStack          EQU     0X1000000
numOfGdts           EQU     0x0000004    
;EndFunctionBlock		各段基地址


;BeginFunctionBlock		GDT
;                                    			段基址                段长          属性
LABEL_GDT        :		Descriptor					0,     		        0,		     0
LABEL_DESC_CODE32:		Descriptor	  KERNEL_SEG_BASE,             0ffffh,         SegDesc_Property_32 |SegDesc_Property_EXEC_R
LABEL_DESC_DATA  :      Descriptor   	DATA_SEG_BASE,            0fffffh,         SegDesc_Property_32 |SegDesc_Property_4KB|SegDesc_Property_RW
LABEL_DESC_STACK :      Descriptor                  0,            0fffffh,         SegDesc_Property_32 |SegDesc_Property_4KB|SegDesc_Property_RW

GdtLen       equ            $-LABEL_GDT                         ;GDT 长度
GdtPtr                      dw GdtLen - 1                       ;GDT 界限
                               dd 0                             ;GDT 基址
;EndFunctionBlock		GDT


;BeginFunctionBlock		GDT 选择子
SelectorCode32     equ         LABEL_DESC_CODE32 -LABEL_GDT     ;代码段选择子
SelectorData       equ         LABEL_DESC_DATA   -LABEL_GDT     ;数据段选择子
SelectorStack      equ         LABEL_DESC_STACK  -LABEL_GDT     ;堆栈段选择子
;EndFunctionBlock		GDT 选择子

LABEL_BEGIN:
        ;BeginFunctionBlock     存放启动信息，进入kernel后用C语言读取,
		MOV		AX,0x0700
		MOV		ES,AX
		MOV		ECX,[ES:0]
		MOV		[ES:12d],ECX                         ;boot_info->length_mp
	    MOV		DWORD[ES:8d ],1024d                  ;boot_info->screen_width
		MOV		DWORD[ES:4d ],768d                   ;boot_info->screen_height
		MOV		DWORD[ES:16d],KERNEL_SEG_BASE        ;boot_info->codeBase
													 ;boot_info->dataBase 20d
		MOV		DWORD[ES:24d],LABEL_GDT              ;boot_info->label_gdt
		MOV		DWORD[ES:28d],numOfGdts              ;boot_info->numOfGdts
		;EndFunctionBlock       存放启动信息，进入kernel后用C语言读取
		MOV		AX,CS
		MOV 	DS,AX
		MOV		ES,AX
		MOV		SS,AX
		MOV 	SP,0

		;BeginFunctionBlock		设置画面显示模式           1024*768*8bit	
		MOV		AX,0x9000
		MOV		ES,AX
		MOV		DI,0
		MOV		AX,0x4f00
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320


		MOV		AX,[ES:DI+4]
		CMP		AX,0x0200
		JB		scrn320

		MOV		CX,0x105
		MOV		AX,0x4f01
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320

		CMP		BYTE [ES:DI+0x19],8
		JNE		scrn320
		CMP		BYTE [ES:DI+0x1b],4
		JNE		scrn320
		MOV		AX,[ES:DI+0x00]
		AND		AX,0x0080
		JZ		scrn320	
		;BeginFunctionBlock		设置视频段基地址
		MOV		EAX,[ES:DI+0x28] 
		MOV		EDX,EAX
		MOV		AX,0x0700
		MOV		ES,AX
		MOV		[ES:0],EDX      ;VRAM BASE 0X7000
		;MOV		AX,CS
		;MOV		ES,AX
		;MOV		EAX,EDX
		;MOV		WORD [LABEL_DESC_VIDEO+2],AX
		;SHR		EAX,16D
		;MOV		BYTE [LABEL_DESC_VIDEO+4],AL
		;MOV     BYTE [LABEL_DESC_VIDEO+7],AH
		;EndFunctionBlock 		设置视频段基地址
		

		MOV		BX,0x4105
		MOV		AX,0x4f02
		INT		0x10
		JMP		SUCCEED

scrn320:
		MOV		AL,0x13
		MOV		AH,0x00
		INT		0x10
		;EndFunctionBlock		设置画面显示模式           1024*768*8bit
		
SUCCEED:	
		;BeginFunctionBlock		载入GDT,打开地址线A20，连续的地址空间
		XOR 	EAX,EAX
		MOV		AX, CS
		SHL		EAX,4D
		ADD 	EAX,LABEL_GDT
		MOV 	DWORD[GdtPtr+2],EAX
		LGDT   [GdtPtr]
		CLI
		IN		AL,92H
		OR		AL,00000010B
		OUT		92H,AL
		
		MOV		EAX,CR0
		OR		EAX,0X00000001
		MOV		CR0,EAX
		;EndFunctionBlock		载入GDT,打开地址线A20，连续的地址空间
		
		;BeginFunctionBlock		跳入32位代码，进入保护模式
		JMP     pipelineflush
pipelineflush:
		MOV		AX,SelectorData
		MOV 	GS,AX
		MOV		DS,AX
		MOV		ES,AX
		MOV		FS,AX
		MOV		AX,SelectorStack
		MOV		SS,AX
		MOV		ESP,topOfStack
		;将内核拷贝到LABEL_DESC_CODE32段中
		setMcpySrcDescNum LABEL_SEG_CODE32,KERNEL_SEG_BASE,KERNEL_SIZE
		CALL	memcpy
		;
		
		;
		MOV		EBX,KERNEL_SEG_BASE
		XOR		ECX,ECX
		MOV		ECX,[EBX+16]    ;kernel中的数据字节数
		CMP		ECX,0
		JE		skip			;
		MOV		ESI,[EBX+20]	;kernel数据在kernel.hrb中的偏移
		ADD		ESI,EBX
		MOV		EDI,[EBX+12]	;将数据放在指定位置
		MOV	    [0X7014],EDI    ;dataBase(boot_info->dataBase)
		CALL	memcpy
skip:	
		JMP		DWORD SelectorCode32:0x0000001b
		;EndFunctionBlock		跳入32位代码，进入保护模式
;begin		函数

memcpy:
		MOV		AL,[ESI]
		MOV		[EDI],AL
		ADD		ESI,1
		ADD		EDI,1
		SUB		ECX,1
		JNZ		memcpy			;
		RET
;end		函数
[bits 32]
LABEL_SEG_CODE32:
;		