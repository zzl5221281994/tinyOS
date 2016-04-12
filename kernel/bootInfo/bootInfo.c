#include "F:\work\tolset\tinyOS\kernel\lib\tinyOS.h   "
#include "F:\work\tolset\tinyOS\kernel\graphics\font.h"
#include "bootInfo.h                                  "
extern struct bootInfo boot_info;
extern struct main_gdt       gdt;
extern struct main_idt       idt;

extern void setGdt(u_int8*lgdt);
int init_bootInfo(                                                                  ){
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
int init_gdt     (                                                                  ){
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
int init_idt     (                                                                  ){
	
}