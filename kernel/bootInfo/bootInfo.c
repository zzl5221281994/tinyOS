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
#include "F:\work\tolset\tinyOS\kernel\lib\global.h   "
#include "F:\work\tolset\tinyOS\kernel\lib\string.h   "
#include "F:\work\tolset\tinyOS\kernel\graphics\font.h"
#include "F:\work\tolset\tinyOS\kernel\kernelFun.h    "
#include "bootInfo.h                                  "
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
	for(i=0;i<256;i++){
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
PUBLIC int init_8259A   (                                                                 ){
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
	io_out8(0xa1,0x28); //0x28 IRQ8  中断向量号
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
	
	load_master_maskWord(0xfe);
	io_delay();
	load_slave_maskWord(0xff);
	io_delay();
	return TRUE;
}
