/************************************************************************************
Wed Apr 13 15:35:40 2016

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
#ifndef bootInfo_h
#define bootInfo_h

#define max_gdtDescriptor 256
#define max_idtDescriptor 256
#define memory_Map_Length 20
#define bootInfo_Pointer  0x7000
#define bootInfo_memMap   0x7020
struct main_gdt             {
	u_int8  gdtDescriptor[max_gdtDescriptor][8];
	u_int8  gdt_ptr[6]                         ;
    u_int32   gdtBase                          ;	
	u_int32   gdtLimit                         ;
	u_int32   gdtDescriptor_length             ;
};
struct main_idt             {
	u_int8    idtDescriptor[max_idtDescriptor][8];
	u_int8    idt_ptr[6]                         ;
	u_int32   idtBase                            ;	
	u_int32   idtLimit                           ;
	u_int32   idtDescriptor_length               ;
};
struct addr_range_descriptor{
	u_int32   BaseAddrLow ;
	u_int32   BaseAddrHigh;
	u_int32   lengthLow   ;
	u_int32   lengthHigh  ;
	u_int32   Type        ;
};
struct bootInfo{
	u_int32   *vram                                 ;
	u_int32   screen_height                         ;
	u_int32   screen_width                          ;
	u_int32   mp_ptr_length                         ;
    u_int32   codeBase                              ;
    u_int32   dataBase                              ;
    u_int32   *label_gdt                            ;
    u_int32    numOfGdts                            ;	
    struct   addr_range_descriptor mp[memory_Map_Length] ;
};
extern  int init_bootInfo(                                                             );
extern  int init_gdt     (                                                             );
extern  int init_idt     (                                                             );
extern  int init_8259A   (                                                             );
extern void init_pit     (u_int32 timesPerSecond                                       );
extern void init_mouse   (                                                             );
extern void init_keyboard(                                                             );
extern  int set_pattle   (                                                             );

/*
0x00:黑
0x01:亮深蓝1
0x02:翠绿
0x03:灰绿
0x04:深红
0x05:深红紫
0x06:暗黄
0x07:白灰
0x08:暗深蓝
0x09:亮深蓝2
0x0a:浅青绿
0x0b:浅天蓝
0x0c:紫红
0x0d:亮紫
0x0e:灰黄
0x0f:白兰
0x10:深翠绿
*/
#endif
