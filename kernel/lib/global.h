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
;�洢����������8�ֽ�

;  BYTE7         BYTE6 BYTE5                               BYTE4 BYTE3 BYTE2    BYTE1 BYTE0
; �λ�ַ2      /   ������    \                              |    �λ�ַ1    |      �ν���
;  31~24      /               \                                23  ~    0           15~0
;            /                 \
;           /                   \
;         7   6   5  4       3 2 1 0    7  6 5  4  3 2 1 0
;         G D/B   0  AVL    |�ν���2|   P  DPL  S   TYPE
;                             19~16
;Pλ������λ���Ƿ����ڴ棬1��ʾ����     Gλ���ν������ȣ�0��ʾ����Ϊ�ֽڣ�1��ʾ����Ϊ4KB
;DPL����Ȩ����0~3 ��ԽС��Ȩ��Խ��      Sλ��S=1��ʾ���ݶ�/����Σ�S=0��ʾϵͳ��/��������
;D/B��   
;*****************************************************************************************/
//;�洢������������
#define SegDesc_Property_4KB                    0x8000  //;4KB����
#define SegDesc_Property_32                     0x4000  //;32λ��

#define SegDesc_Property_DPL0                   0x0000  //;DPL����Ȩ��=0
#define SegDesc_Property_DPL1                   0x0020  //;DPL����Ȩ��=1
#define SegDesc_Property_DPL2                   0x0040  //;DPL����Ȩ��=2
#define SegDesc_Property_DPL3                   0x0060  //;DPL����Ȩ��=3

#define SegDesc_Property_R                      0x0090  //;���ڵ�ֻ�����ݶ�	
#define SegDesc_Property_RW                     0x0092  //;���ڵĿɶ�д���ݶ�
#define SegDesc_Property_RWA                    0x0093  //;���ڵ��ѷ��ʿɶ�д���ݶ�	
#define SegDesc_Property_EXEC                   0x0098  //;���ڵ�ִֻ�д����	
#define SegDesc_Property_EXEC_R                 0x009A  //;���ڵĿ�ִ�пɶ������		
#define SegDesc_Property_EXEC_C                 0x009C  //;���ڵĿ�ִ��һ�´����	
#define SegDesc_Property_EXEC_RC                0x009E  //;���ڵĿɶ���ִ��һ�´����	
//;ϵͳ��������
#define SegDesc_Property_LDT                    0x0082  //;���ڵľֲ����������
#define SegDesc_Property_TaskGate               0x0085  //;���ڵ������Ŷ�
#define SegDesc_Property_386TSS                 0x0089  //;���ڵ�386����״̬��
#define SegDesc_Property_386CGate               0x008C  //;���ڵ�386�����Ŷ�
#define SegDesc_Property_386IGate               0x008E  //;���ڵ�386�ж��Ŷ�
#define SegDesc_Property_TGate                  0x008F  //;���ڵ�386�����Ŷ�
/*****************************************************************************************
CRO:
|----------------------------------------------------------------------|
|   31 30 29--------------18 17 16----------------5  4  3  2  1  0     |
|   PG CD NW              AM    WP                NE ET TS EM MP PE    |
|----------------------------------------------------------------------|
******************************************************************************************/


/*****************************************************************************************
; ѡ����ͼʾ:
;         �������ש����ש����ש����ש����ש����ש����ש����ש����ש����ש����ש����ש����ש����ש����ש�����
;         �� 15 �� 14 �� 13 �� 12 �� 11 �� 10 �� 9  �� 8  �� 7  �� 6  �� 5  �� 4  �� 3  �� 2  �� 1  �� 0  ��
;         �ǩ����ߩ����ߩ����ߩ����ߩ����ߩ����ߩ����ߩ����ߩ����ߩ����ߩ����ߩ����ߩ����贈���贈���ߩ�����
;         ��                                 ����������                                 �� TI ��   RPL    ��
;         �������������������������������������������������������������������������������ߩ����ߩ�����������
;
; RPL(Requested Privilege Level): ������Ȩ����������Ȩ��顣
;
; TI(Table Indicator): ������������ָʾλ
;	TI=0 ָʾ��ȫ����������GDT�ж�ȡ��������
;	TI=1 ָʾ�Ӿֲ���������LDT�ж�ȡ��������
;

;----------------------------------------------------------------------------
; ѡ��������ֵ˵��
; ����:
*****************************************************************************************/

#define Selector_RPL0			0//	; ��
#define Selector_RPL1			1//	; �� RPL
#define Selector_RPL2			2//	; ��
#define Selector_RPL3			3//	; ��

#define Selector_TI_Gdt			0//	; ��TI
#define Selector_TI_Ldt			4//	; ��

#define KERNEL_CODE_INDEX       1
#define KERNEL_DATA_INDEX       2
#define KERNEL_STACK_INDEX      3
//;----------------------------------------------------------------------------
//ȫ��ʹ�õĳ���,�����ĺ궨��
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
