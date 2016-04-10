#include "F:\work\tolset\tinyOS\kernel\lib\tinyOS.h   "
#include "F:\work\tolset\tinyOS\kernel\graphics\font.h"
#include "bootInfo.h                                  "
extern struct bootInfo boot_info;
extern struct main_gdt       gdt;
extern struct main_idt       idt;

extern void setGdt(unsigned char*lgdt);
int init_bootInfo(                                                                  ){
    unsigned char*bp=(unsigned char*)bootInfo_Pointer,*mp_ptr=(unsigned char*)bootInfo_memMap;
	boot_info.vram         = (unsigned int*)(*(unsigned int*)(bp+0));
	boot_info.screen_height=*(unsigned int*)(bp+4 );
	boot_info.screen_width =*(unsigned int*)(bp+8 );
	boot_info.mp_ptr_length=*(unsigned int*)(bp+12);
	boot_info.codeBase     =*(unsigned int*)(bp+16);
	boot_info.dataBase     =*(unsigned int*)(bp+20);
	boot_info.label_gdt    = (unsigned int*)(*(unsigned int*)(bp+24));
	boot_info.numOfGdts    =*(unsigned int*)(bp+28);
	if(boot_info.mp_ptr_length>=memory_Map_Length)
		return FALSE;
	int i;
	for(i=0;i<boot_info.mp_ptr_length;i++)
	{
		boot_info.mp[i].BaseAddrLow  =*(unsigned int*)(mp_ptr+0 );
		boot_info.mp[i].BaseAddrHigh =*(unsigned int*)(mp_ptr+4 );
		boot_info.mp[i].lengthLow    =*(unsigned int*)(mp_ptr+8 );
		boot_info.mp[i].lengthHigh   =*(unsigned int*)(mp_ptr+12);
		boot_info.mp[i].Type         =*(unsigned int*)(mp_ptr+16);
		mp_ptr+=20;
	}
	return TRUE;
}
int init_gdt     (                                                                  ){
	unsigned char*ptr=(unsigned char*)boot_info.label_gdt;
	int num          =boot_info.numOfGdts                ;
	int i;
	if(num>max_gdtDescriptor)
		return FALSE;
	for(i=0;i<num;i++)
	{
		memcpy8(ptr,(unsigned char*)(&(gdt.gdtDescriptor[i])),8);
		ptr=ptr+8;
	}
	gdt.gdtDescriptor_length=num;
	unsigned int limit=256*8-1;
	unsigned int base =gdt.gdtDescriptor    ;
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