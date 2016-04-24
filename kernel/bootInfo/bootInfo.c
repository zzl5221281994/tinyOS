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
#include "F:\work\tolset\tinyOS\kernel\lib\global.h             "
#include "F:\work\tolset\tinyOS\kernel\lib\string.h             "
#include "F:\work\tolset\tinyOS\kernel\graphics\font.h          "
#include "F:\work\tolset\tinyOS\kernel\kernelFun.h              "
#include "F:\work\tolset\tinyOS\kernel\interrupt\interrupt.h    "
#include "F:\work\tolset\tinyOS\kernel\debug\debug.h            "
#include "bootInfo.h                                            "
#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4
#define PORT_KEYDAT				0x0060
#define PORT_KEYSTA				0x0064
#define PORT_KEYCMD				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47

extern void   hander();
PUBLIC int init_bootInfo(                                                                  ){
    u_int8*bp=(u_int8*)bootInfo_Pointer,*mp_ptr=(u_int8*)bootInfo_memMap;
	boot_info.vram         = (u_int32*)(*(u_int32*)(bp+0));
	boot_info.screen_height=*(u_int32*)(bp+4 );
	boot_info.screen_width =*(u_int32*)(bp+8 );
	boot_info.mp_ptr_length=*(u_int32*)(bp+12);
	boot_info.codeBase     =*(u_int32*)(bp+16);
	boot_info.dataBase     =*(u_int32*)(bp+20);
	boot_info.label_gdt    = (u_int32*)(*(u_int32*)(bp+24));
	boot_info.numOfGdts    =*(u_int32*)(bp+28);
	if(boot_info.mp_ptr_length>=memory_Map_Length)
		return FALSE;
	int i;
	for(i=0;i<boot_info.mp_ptr_length;i++)
	{
		boot_info.mp[i].BaseAddrLow  =*(u_int32*)(mp_ptr+0 );
		boot_info.mp[i].BaseAddrHigh =*(u_int32*)(mp_ptr+4 );
		boot_info.mp[i].lengthLow    =*(u_int32*)(mp_ptr+8 );
		boot_info.mp[i].lengthHigh   =*(u_int32*)(mp_ptr+12);
		boot_info.mp[i].Type         =*(u_int32*)(mp_ptr+16);
		mp_ptr+=20;
	}
	return TRUE;
}
PUBLIC int init_gdt     (                                                                  ){
	u_int8*ptr=(u_int8*)boot_info.label_gdt;
	int num          =boot_info.numOfGdts                ;
	int i;
	if(num>max_gdtDescriptor)
		return FALSE;
	for(i=0;i<num;i++)
	{
		memcpy8(ptr,(u_int8*)(&(gdt.gdtDescriptor[i])),8);
		ptr=ptr+8;
	}
	gdt.gdtDescriptor_length=num;
	u_int32 limit=256*8-1;
	u_int32 base =gdt.gdtDescriptor    ;
	gdt.gdt_ptr[0]=(limit&0x000000ff);
	gdt.gdt_ptr[1]=((limit&0x0000ff00)>>8);
	gdt.gdt_ptr[2]=(base &0x000000ff);
	gdt.gdt_ptr[3]=((base &0x0000ff00)>>8);
	gdt.gdt_ptr[4]=((base &0x00ff0000)>>16);
	gdt.gdt_ptr[5]=((base &0xff000000)>>24);
	gdt.gdtBase=base;
	gdt.gdtLimit=limit;
	setGdt(gdt.gdt_ptr);
	return TRUE;
}
PUBLIC int init_idt     (                                                                  ){
	u_int8 desc[8];
	int i;
	//int0~int19                   CPU异常
	for(i=0;i<20;i++){
		gen_gateDescriptor(desc,8,exception_hander[i],0,SegDesc_Property_386IGate);
		memcpy8(desc,(u_int8*)(&(idt.idtDescriptor[i])),8);
	}
	//int20~int 0x1f               intel保留
	for(i=20;i<0x20;i++){
		gen_gateDescriptor(desc,8,hander,0,SegDesc_Property_386IGate);
		memcpy8(desc,(u_int8*)(&(idt.idtDescriptor[i])),8);
	}
	//int 0x20~int0x2f             可屏蔽硬件中断，来自8259A
	for(i=0x20;i<=0x2f;i++){
		gen_gateDescriptor(desc,8,interrupt_hander[i-0x20],0,SegDesc_Property_386IGate);
		memcpy8(desc,(u_int8*)(&(idt.idtDescriptor[i])),8);
	}
	//int 0x30~ int 0x87           默认
	for(i=0x30;i<0x88;i++){
		gen_gateDescriptor(desc,8,hander,0,SegDesc_Property_386IGate);
		memcpy8(desc,(u_int8*)(&(idt.idtDescriptor[i])),8);
	}
	//int 0x88                     系统调用
	gen_gateDescriptor(desc,8,sys_call,0,SegDesc_Property_386IGate|SegDesc_Property_DPL3);
	memcpy8(desc,(u_int8*)(&(idt.idtDescriptor[0x88])),8);
	
	//int 0x89~ int 255            默认
	for(i=0x88+1;i<256;i++){
		gen_gateDescriptor(desc,8,hander,0,SegDesc_Property_386IGate);
		memcpy8(desc,(u_int8*)(&(idt.idtDescriptor[i])),8);
	}
	u_int32 limit=256*8-1              ;
	u_int32 base =idt.idtDescriptor    ;
	idt.idtBase=base                   ;
	idt.idtLimit=limit                 ;
	
	idt.idt_ptr[0]=(limit&0x000000ff);
	idt.idt_ptr[1]=((limit&0x0000ff00)>>8);
	idt.idt_ptr[2]=(base &0x000000ff);
	idt.idt_ptr[3]=((base &0x0000ff00)>>8);
	idt.idt_ptr[4]=((base &0x00ff0000)>>16);
	idt.idt_ptr[5]=((base &0xff000000)>>24);
	setIdt(idt.idt_ptr);
	return TRUE;
}
PUBLIC int  init_8259A   (                                                                 ){
	//ICW1 master
	io_out8(0x20,0x11);
	io_delay();
	//ICW1 slave
	io_out8(0xa0,0x11);
	io_delay();
	
	//ICW2 master
	io_out8(0x21,0x20);  //0x20 IRQ0 中断向量号
	io_delay(); 
	//ICW2 slave        
	io_out8(0xa1,0x28); //0x28  IRQ8 中断向量号
	io_delay();
	
	//ICW3 master
	io_out8(0x21,0x04); //IRQ2 对应 slave 8259A
	io_delay();
	//ICW3 slave
	io_out8(0xa1,0x02); //对应master 8259A 的 IRQ2
	io_delay();
	
	//ICW4 master
	io_out8(0x21,0x01);
	io_delay();
	//ICW4 slave
	io_out8(0xa1,0x01);
	io_delay();
	
	load_master_maskWord(0x00);
	io_delay();
	load_slave_maskWord(0x00);
	io_delay();
	return TRUE;
}
PUBLIC void init_pit     (u_int32 timesPerSecond                                           ){
	io_out8(0x43,0x34);
	u_int32 value=1193180/timesPerSecond;
	io_out8(0x40,value&0x000000ff);
	io_delay();
	io_out8(0x40,((value&0x0000ff00)>>8));
	return;
}
PUBLIC void init_mouse   (                                                                 ){
	io_delay();
	io_out8(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
	io_delay();
	io_out8(PORT_KEYDAT, MOUSECMD_ENABLE);
	return; 
}
PUBLIC void init_keyboard(                                                                 ){
	io_delay();
	io_out8(PORT_KEYCMD, KEYCMD_WRITE_MODE);
	io_delay();
	io_out8(PORT_KEYDAT, KBC_MODE);
	return;
}