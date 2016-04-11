/***********************************************************************************
Wed Mar 30 22:50:57 2016

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
#include "graphics\font.h    "
#include "lib\tinyOS.h       "
#include "bootInfo\bootInfo.h"
extern void io_hlt(void) ;
struct bootInfo boot_info;
struct main_gdt       gdt;
struct main_idt       idt;
u_int8*vram8 =NULL;
u_int32 *vram32=NULL;
//static u_int8* tinyOS_str1="\nWed Mar 30 22 50 57 2016\n\nMIT License\nCopyright c 2016 zhuzuolang\n\nPermission is hereby granted free of charge to any person obtaining a copy\nof this software and associated documentation files the Software to deal\nin the Software without restriction including without limitation the rights\nto use copy modify merge publish distribute sublicense and or sell\ncopies of the Software and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions\nThe above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software \nTHE SOFTWARE IS PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY WHETHER IN AN ACTION OF CONTRACT TORT OR OTHERWISE ARISING FROM \nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE\n\nWed Mar 30 22 50 57 2016\n\nMIT License\nCopyright c 2016 zhuzuolang\n\nPermission is hereby granted free of charge to any person obtaining a copy\nof this software and associated documentation files the Software to deal\nin the Software without restriction including without limitation the rights\nto use copy modify merge publish distribute sublicense and or sell\ncopies of the Software and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions\nThe above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software \nTHE SOFTWARE IS PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY WHETHER IN AN ACTION OF CONTRACT TORT OR OTHERWISE ARISING FROM \nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE\n";
static u_int8* errorMessage="init bootInfo error\n";
void test(u_int32 a,u_int32 b,int32(*cmp)(u_int32,u_int32)){
	int32 i,j;
	int32 num;
	if(cmp(a,b)==1)
		num=500;
	else
		num=184;
	//drawNum(num,400,0,0x21,0x3c);
	for(j=0;j<gdt.gdtDescriptor_length;j++)
	{
	 u_int8*str=gdt.gdtDescriptor[j];
	for(i=0;i<8;i++)
	{
		u_int32 key=str[i];
		drawNum(key,num+j*16,i*30,0x21,0x3c);
	}
	}
}
int32 cmp(u_int32 a,u_int32 b)
{
	return a>b?1:0;
}
int32 addDes(u_int32 base,u_int32 limit,u_int32 attribute){
	u_int8 desc[8];
	gen_normalDescriptor(desc,base,limit,attribute);
	memcpy8(desc,(u_int8*)(&(gdt.gdtDescriptor[gdt.gdtDescriptor_length])),8);
	gdt.gdtDescriptor_length++;
	return gdt.gdtDescriptor_length-1;
}
void drawInfo();
extern void callGate_enter(u_int32 Selector);
//extern void callGate_return();
extern void callGate_test();
void HariMain(void)
{
	init_bootInfo();
	init_gdt();
	vram8 =(u_int8*)boot_info.vram;
	vram32=(u_int32* )boot_info.vram;
	test(1,2,cmp);
	int32 index=addDes(boot_info.codeBase,0xffff,SegDesc_Property_32 |SegDesc_Property_EXEC_R);
	
	u_int8 desc[8];
	gen_gateDescriptor(desc,index*8,callGate_test,0,SegDesc_Property_386CGate|SegDesc_Property_DPL0);
	memcpy8(desc,(u_int8*)(&(gdt.gdtDescriptor[gdt.gdtDescriptor_length])),8);
	gdt.gdtDescriptor_length++;
	callGate_enter(8*(gdt.gdtDescriptor_length-1));
	drawInfo();
	test(2,1,cmp);
	while(1)
		io_hlt();
}
void drawInfo(){
	drawStr("    vram base",0,8,0x3c,0x00);
	drawNum((u_int32)boot_info.vram,0,200,0x2e,0x00);
	drawStr("screen height",16,0,0x3c,0x00);
	drawNum(boot_info.screen_height,16,200,0x2e,0x00);
	drawStr(" screen width",32,0,0x3c,0x00);
	drawNum(boot_info.screen_width,32,200,0x2e,0x00);
	drawStr("    mp length",48,0,0x3c,0x00);
	drawNum(boot_info.mp_ptr_length,48,200,0x2e,0x00);
	drawStr("     codeBase",64,0,0x3c,0x00);
	drawNum(boot_info.codeBase,64,200,0x2e,0x00);
	drawStr("     dataBase",80,0,0x3c,0x00);
	drawNum(boot_info.dataBase,80,200,0x2e,0x00);
	
	drawStr("old label gdt",96,0,0x3c,0x00);
	drawNum(boot_info.label_gdt,96,200,0x2e,0x00);
	drawStr("new label gdt",96,300,0x3c,0x00);
	drawNum(gdt.gdtBase,96,450,0x2e,0x00);
	drawStr("new gdt limit",96,650,0x3c,0x00);
	drawNum(gdt.gdtLimit,96,800,0x2e,0x00);
	drawStr("old numOfGdts",112,0,0x3c,0x00);
	drawNum(boot_info.numOfGdts,112,200,0x2e,0x00);
	drawStr("BaseAddrLow  BaseAddrHigh  lengthLow  lengthHigh  Type",0,400,0X2F,0X00);
	int32 i;
	for(i=0;i<boot_info.mp_ptr_length;i++)
	{
		drawNum(boot_info.mp[i].BaseAddrLow ,(i+1)*16,400,0x1f,0x00);
		drawNum(boot_info.mp[i].BaseAddrHigh,(i+1)*16,400+13*8,0x1f,0x00);
		drawNum(boot_info.mp[i].lengthLow   ,(i+1)*16,400+27*8,0x1f,0x00);
		drawNum(boot_info.mp[i].lengthHigh  ,(i+1)*16,400+38*8,0x1f,0x00);
		drawNum(boot_info.mp[i].Type        ,(i+1)*16,400+50*8,0x1f,0x00);
	}
	return ;
}