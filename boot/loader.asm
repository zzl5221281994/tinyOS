%include "boot\boot_parameter.inc"
%include "boot\boot_macro.inc    "
org 0x7e00
;%include "boot\memory_map.inc    ";此文件包含获取内存分布图的代码，并将得到的内存分布图数据结构保存在0x70014h开始的内存里
	JMP		LABEL_BEGIN
[SECTION .GDT]
;GDT begin
;                                    段基址                段长          属性
LABEL_GDT        :		Descriptor	        0,     		       0,		     0
LABEL_DESC_CODE32:		Descriptor          0,             0ffffh,         SegDesc_Property_32|SegDesc_Property_EXEC_R
LABEL_DESC_VIDEO :		Descriptor    0A0000h,             0ffffh,         SegDesc_Property_RW
LABEL_DESC_STACK :      Descriptor    080000h,             0ffffh,         SegDesc_Property_32|SegDesc_Property_RWA
LABEL_DESC_DATA  :      Descriptor   0500000h,             0ffffh,         SegDesc_Property_RW
LABEL_DESC_MEMMAP:		Descriptor    070000h,			   0ffffh,		   SegDesc_Property_RW
;GDT end

GdtLen       equ            $-LABEL_GDT                         ;GDT 长度
GdtPtr                      dw GdtLen - 1                       ;GDT 界限
                               dd 0                             ;GDT 基址

;GDT 选择子
SelectorCode32     equ         LABEL_DESC_CODE32 -LABEL_GDT     ;代码段选择子
SelectorVideo      equ         LABEL_DESC_VIDEO  -LABEL_GDT     ;视频段选择子
SelectorStack      equ         LABEL_DESC_STACK  -LABEL_GDT     ;堆栈段选择子
SelectorData       equ         LABEL_DESC_DATA   -LABEL_GDT     ;数据段选择子
SelectorMemMap	   equ		   LABEL_DESC_MEMMAP -LABEL_GDT     ;内存分布图段选择子

[SECTION .s16]
[BITS      16]
LABEL_BEGIN:
		MOV		AX,CS
		MOV 	DS,AX
		MOV		ES,AX
		MOV		SS,AX
		MOV 	SP,0

		;设置画面显示模式           1024*768*8bit	
		MOV		AX,0x9000
		MOV		ES,AX
		MOV		DI,0
		MOV		AX,0x4f00
		INT		0x10
		CMP		AX,0x004f
		JNE		scrn320


		MOV		AX,[ES:DI+4]
		CMP		AX,0x0200
		JB		scrn320			; if (AX < 0x0200) goto scrn320

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
		MOV		EAX,[ES:DI+0x28]            ;设置视频段基地址
		MOV		WORD [LABEL_DESC_VIDEO+2],AX
		SHR		EAX,16D
		MOV		BYTE [LABEL_DESC_VIDEO+4],AL
		MOV     BYTE [LABEL_DESC_VIDEO+7],AH
		

		MOV		BX,0x4105
		MOV		AX,0x4f02
		INT		0x10
		JMP		SUCCEED

scrn320:
		MOV		AL,0x13
		MOV		AH,0x00
		INT		0x10
		
SUCCEED:
		;设置数据段基址
		XOR		EAX,EAX
		MOV		AX,CS
		SHL		EAX,4D
		ADD 	EAX,LABEL_SEG_CODE32
		MOV		WORD [LABEL_DESC_DATA+2],AX
		SHR		EAX,16D
		MOV		BYTE [LABEL_DESC_DATA+4],AL
		MOV     BYTE [LABEL_DESC_DATA+7],AH
		;设置代码段基址
		XOR 	EAX,EAX
		MOV		AX,CS
		SHL		EAX,4D
		ADD 	EAX,LABEL_SEG_CODE32
		MOV		WORD [LABEL_DESC_CODE32+2],AX
		SHR		EAX,16D
		MOV		BYTE [LABEL_DESC_CODE32+4],AL
		MOV     BYTE [LABEL_DESC_CODE32+7],AH
		
		;设置堆栈段基址
		;XOR 	EAX,EAX
		;MOV		AX,CS
		;SHL		EAX,4D
		;ADD 	EAX,LABEL_SEG_CODE32
		;MOV		WORD [LABEL_DESC_STACK+2],AX
		;SHR		EAX,16D
		;MOV		BYTE [LABEL_DESC_STACK+4],AL
		;MOV     BYTE [LABEL_DESC_STACK+7],AH
		;载入GDT
		XOR 	EAX,EAX
		MOV		AX, CS
		SHL		EAX,4D
		ADD 	EAX,LABEL_GDT
		MOV 	DWORD[GdtPtr+2],EAX
		LGDT   [GdtPtr]
		;打开地址线A20，连续的地址空间
		CLI
		IN		AL,92H
		OR		AL,00000010B
		OUT		92H,AL
		
		MOV		EAX,CR0
		OR		EAX,0X00000001
		MOV		CR0,EAX
		;跳入32位代码，进入保护模式
		JMP     pipelineflush
pipelineflush:
		MOV		AX,SelectorVideo
		MOV 	GS,AX
		MOV		AX,SelectorStack
		MOV		SS,AX
		MOV		SP,0XFFFC
		MOV		AX,SelectorData
		MOV		DS,AX
		JMP		DWORD SelectorCode32:0
;end  s16 section
[SECTION .S32]
[BITS      32]
LABEL_SEG_CODE32:
;		