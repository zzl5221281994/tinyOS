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
#include "graphics\font.h      "
#include "lib\global.h         "
#include "lib\string.h         "
#include "bootInfo\bootInfo.h  "
#include "kernelFun.h          "
#include "interrupt\interrupt.h"
#include "multiTask\process.h  "
#include "IO\IO.h              "
#include "drivers\hd.h         "
#include "debug\debug.h        "
#include "interrupt\sys_call.h "
#define  MAX_SERVER_SIZE     20480
PUBLIC struct bootInfo boot_info;
PUBLIC struct main_gdt       gdt;
PUBLIC struct main_idt       idt;
PUBLIC u_int8  *vram8 =NULL     ;
PUBLIC u_int32 *vram32=NULL     ;
PUBLIC u_int32 time=0           ;
//static u_int8* tinyOS_str1="\nWed Mar 30 22 50 57 2016\n\nMIT License\nCopyright c 2016 zhuzuolang\n\nPermission is hereby granted free of charge to any person obtaining a copy\nof this software and associated documentation files the Software to deal\nin the Software without restriction including without limitation the rights\nto use copy modify merge publish distribute sublicense and or sell\ncopies of the Software and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions\nThe above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software \nTHE SOFTWARE IS PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY WHETHER IN AN ACTION OF CONTRACT TORT OR OTHERWISE ARISING FROM \nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE\n\nWed Mar 30 22 50 57 2016\n\nMIT License\nCopyright c 2016 zhuzuolang\n\nPermission is hereby granted free of charge to any person obtaining a copy\nof this software and associated documentation files the Software to deal\nin the Software without restriction including without limitation the rights\nto use copy modify merge publish distribute sublicense and or sell\ncopies of the Software and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions\nThe above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software \nTHE SOFTWARE IS PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY WHETHER IN AN ACTION OF CONTRACT TORT OR OTHERWISE ARISING FROM \nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE\n";
void drawInfo();
u_int16 hd_buf[MAX_SERVER_SIZE/2];
u_int16 fs_buf[MAX_SERVER_SIZE/2];
u_int16 wd_buf[MAX_SERVER_SIZE/2];
/*内核入口*/
void HariMain(void)
{
	init_bootInfo();                  //启动信息
	vram8 =(u_int8*)boot_info.vram  ;
	vram32=(u_int32* )boot_info.vram;
	init_gdt();
	init_idt();
	init_8259A(); 
    init_keyboard();
    init_mouse();	
	init_pit(100);                    //设置每秒时钟中断次数
	init_tss();	
	init_msg_queue();
	init_sys_call_table();
	init_process_table();
	u_int8*ptr=0x38940;
	memcpy8(ptr,(u_int8*)hd_buf,MAX_SERVER_SIZE);
	ptr=ptr+MAX_SERVER_SIZE;
	memcpy8(ptr,(u_int8*)fs_buf,MAX_SERVER_SIZE);
	ptr+=MAX_SERVER_SIZE;
	memcpy8(ptr,(u_int8*)wd_buf,MAX_SERVER_SIZE);
	
	//drawNum(1234,100,100,0x3c,0x00);
	createServer((u_int8*)hd_buf,MAX_SERVER_SIZE,1,1);
	createServer((u_int8*)fs_buf,MAX_SERVER_SIZE,1,1);
	createServer((u_int8*)wd_buf,MAX_SERVER_SIZE,1,1);
	open_interrupt();
	while(1)
		io_hlt();
}
void testB(){
	u_int32 l=0;
	while(1)
	{
		*(vram8+l)=0x3c;
		int i,j,key=0;
	for(i=0;i<10;i++)
		for(j=0;j<1000;j++)
			   key+=i+j;
		l++;
	}
}
void testA(){
	u_int32 l=0;
	while(1)
	{
		*(vram8+l)=0x1f;
		int i,j,key=0;
	for(i=0;i<10;i++)
		for(j=0;j<1000;j++)
			   key+=i+j;
		l++;
	}
}
void drawInfo(){
	drawStr("    vram base",0,8,0x3c,0x00  );
	drawNum((u_int32)boot_info.vram,0,200,0x2e,0x00);
	drawStr("screen height",16,0,0x3c,0x00 );
	drawNum(boot_info.screen_height,16,200,0x2e,0x00);
	drawStr(" screen width",32,0,0x3c,0x00 );
	drawNum(boot_info.screen_width,32,200,0x2e,0x00);
	drawStr("    mp length",48,0,0x3c,0x00 );
	drawNum(boot_info.mp_ptr_length,48,200,0x2e,0x00);
	drawStr("     codeBase",64,0,0x3c,0x00 );
	drawNum(boot_info.codeBase,64,200,0x2e,0x00);
	drawStr("     dataBase",80,0,0x3c,0x00 );
	drawNum(boot_info.dataBase,80,200,0x2e,0x00);
	
	drawStr("old label gdt",96,0,0x3c,0x00 );
	drawNum((u_int32)boot_info.label_gdt,96,200,0x2e,0x00);
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
//drawStr("~!@#$%^&*()_+}{|\":?><",200,0,0x1f,0x00);
	return ;
}