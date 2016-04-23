/************************************************************************************
Wed Apr 13 15:36:43 2016

MIT License
Copyright (c) 2016 zhuzuolang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************************/
#ifndef global_H
#define global_H
#include "type.h                                                 "
/******************************************************************************************
;存储段描述符共8字节

;  BYTE7         BYTE6 BYTE5                               BYTE4 BYTE3 BYTE2    BYTE1 BYTE0
; 段基址2      /   段属性    \                              |    段基址1    |      段界限
;  31~24      /               \                                23  ~    0           15~0
;            /                 \
;           /                   \
;         7   6   5  4       3 2 1 0    7  6 5  4  3 2 1 0
;         G D/B   0  AVL    |段界限2|   P  DPL  S   TYPE
;                             19~16
;P位：存在位，是否在内存，1表示存在     G位：段界限粒度，0表示粒度为字节，1表示粒度为4KB
;DPL：特权级，0~3 ，越小特权级越大      S位：S=1表示数据段/代码段，S=0表示系统段/门描述符
;D/B：   
;*****************************************************************************************/
//;存储段描述符属性
#define SegDesc_Property_4KB                    0x8000  //;4KB粒度
#define SegDesc_Property_32                     0x4000  //;32位段

#define SegDesc_Property_DPL0                   0x0000  //;DPL：特权级=0
#define SegDesc_Property_DPL1                   0x0020  //;DPL：特权级=1
#define SegDesc_Property_DPL2                   0x0040  //;DPL：特权级=2
#define SegDesc_Property_DPL3                   0x0060  //;DPL：特权级=3

#define SegDesc_Property_R                      0x0090  //;存在的只读数据段	
#define SegDesc_Property_RW                     0x0092  //;存在的可读写数据段
#define SegDesc_Property_RWA                    0x0093  //;存在的已访问可读写数据段	
#define SegDesc_Property_EXEC                   0x0098  //;存在的只执行代码段	
#define SegDesc_Property_EXEC_R                 0x009A  //;存在的可执行可读代码段		
#define SegDesc_Property_EXEC_C                 0x009C  //;存在的可执行一致代码段	
#define SegDesc_Property_EXEC_RC                0x009E  //;存在的可读可执行一致代码段	
//;系统段描述符
#define SegDesc_Property_LDT                    0x0082  //;存在的局部描述符表段
#define SegDesc_Property_TaskGate               0x0085  //;存在的任务门段
#define SegDesc_Property_386TSS                 0x0089  //;存在的386任务状态段
#define SegDesc_Property_386CGate               0x008C  //;存在的386调用门段
#define SegDesc_Property_386IGate               0x008E  //;存在的386中断门段
#define SegDesc_Property_TGate                  0x008F  //;存在的386陷阱门段
/*****************************************************************************************
CRO:
|----------------------------------------------------------------------|
|   31 30 29--------------18 17 16----------------5  4  3  2  1  0     |
|   PG CD NW              AM    WP                NE ET TS EM MP PE    |
|----------------------------------------------------------------------|
******************************************************************************************/


/*****************************************************************************************
; 选择子图示:
;         ┏━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┓
;         ┃ 15 ┃ 14 ┃ 13 ┃ 12 ┃ 11 ┃ 10 ┃ 9  ┃ 8  ┃ 7  ┃ 6  ┃ 5  ┃ 4  ┃ 3  ┃ 2  ┃ 1  ┃ 0  ┃
;         ┣━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━╋━━╋━━┻━━┫
;         ┃                                 描述符索引                                 ┃ TI ┃   RPL    ┃
;         ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━┻━━━━━┛
;
; RPL(Requested Privilege Level): 请求特权级，用于特权检查。
;
; TI(Table Indicator): 引用描述符表指示位
;	TI=0 指示从全局描述符表GDT中读取描述符；
;	TI=1 指示从局部描述符表LDT中读取描述符。
;

;----------------------------------------------------------------------------
; 选择子类型值说明
; 其中:
*****************************************************************************************/

#define Selector_RPL0			0//	; ┓
#define Selector_RPL1			1//	; ┣ RPL
#define Selector_RPL2			2//	; ┃
#define Selector_RPL3			3//	; ┛

#define Selector_TI_Gdt			0//	; ┓TI
#define Selector_TI_Ldt			4//	; ┛

#define KERNEL_CODE_INDEX       1
#define KERNEL_DATA_INDEX       2
#define KERNEL_STACK_INDEX      3
//;----------------------------------------------------------------------------
//全局使用的常量,函数的宏定义
#define NULL    0
#define FALSE   0
#define TRUE    1
#define write_video8( add,color) ((*(vram8 +(add)))=(color))
#define write_video32(add,color) ((*(vram32+(add)))=(color))
//glboal bootInfo ,gdt,idt
extern struct bootInfo boot_info;
extern struct main_gdt       gdt;
extern struct main_idt       idt;
//global vram
extern u_int8           *vram8 ;
extern u_int32          *vram32;
extern void gen_normalDescriptor(u_int8 desc[8],u_int32 base    ,u_int32 limit ,u_int32 attribute                         );
extern void   gen_gateDescriptor(u_int8 desc[8],u_int32 Selector,u_int32 offset,u_int32 DCount,u_int32 attribute          );
extern int32 addDes             (u_int32 base,u_int32 limit,u_int32 attribute                                             );
#endif
