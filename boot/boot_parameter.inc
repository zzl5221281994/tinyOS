;/************************************************************************************
;Thu Mar 31 15:04:03 2016
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
;段描述符共8字节

;  BYTE7         BYTE6 BYTE5                               BYTE4 BYTE3 BYTE2    BYTE1 BYTE0
;  段基址2     /   段属性    \                              |    段基址1    |      段界限
;  31~24      /               \                                23  ~    0           15~0
;            /                 \
;           /                   \
;         7   6   5  4       3 2 1 0    7  6 5  4  3 2 1 0
;         G D/B   0  AVL    |段界限2|   P  DPL  S   TYPE
;                             19~16
;P位：存在位，是否在内存，1表示存在     G位：段界限粒度，0表示粒度为字节，1表示粒度为4KB
;DPL：特权级，0~3 ，越小特权级越大      S位：S=1表示数据段/代码段，S=0表示系统段/门描述符
;D/B：   
;
;
;
;
;
;存储段描述符属性
SegDesc_Property_4KB  			EQU		8000H  ;4KB粒度
SegDesc_Property_32				EQU		4000H  ;32位段

SegDesc_Property_DPL0			EQU		0000H  ;DPL：特权级=0
SegDesc_Property_DPL1			EQU		0020H  ;DPL：特权级=1
SegDesc_Property_DPL2			EQU		0040H  ;DPL：特权级=2
SegDesc_Property_DPL3			EQU     0060H  ;DPL：特权级=3

SegDesc_Property_R 				EQU		0090H  ;存在的只读数据段	
SegDesc_Property_RW				EQU		0092H  ;存在的可读写数据段
SegDesc_Property_RWA			EQU     0093H  ;存在的已访问可读写数据段	
SegDesc_Property_EXEC			EQU		0098H  ;存在的只执行代码段	
SegDesc_Property_EXEC_R 		EQU		009AH  ;存在的可执行可读代码段		
SegDesc_Property_EXEC_C			EQU		009CH  ;存在的可执行一致代码段	
SegDesc_Property_EXEC_RC		EQU		009EH  ;存在的可读可执行一致代码段	
;系统段描述符
SegDesc_Property_LDT			EQU		0082H  ;局部描述符表段
SegDesc_Property_TaskGate		EQU		0085H  ;任务门段
SegDesc_Property_386TSS			EQU     0089H  ;386任务状态段
SegDesc_Property_386CGate		EQU		008CH  ;386调用门段
SegDesc_Property_386IGate		EQU		008EH  ;386中断门段
SegDesc_Property_TGate			EQU		008FH  ;386陷阱门段

KERNEL_SIZE						EQU		4000
;内存分布图数据结构入口