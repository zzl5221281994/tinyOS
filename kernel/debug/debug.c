/************************************************************************************
Thu Apr 14 10:11:45 2016

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
#include "F:\work\tolset\tinyOS\kernel\lib\global.h       "
#include "F:\work\tolset\tinyOS\kernel\graphics\font.h    "
#include "F:\work\tolset\tinyOS\kernel\bootInfo\bootInfo.h"
#include "F:\work\tolset\tinyOS\kernel\kernelFun.h        "
#define errorDescribeLength 40
#define FAULT     1
#define TRAP      2
#define ABORT     3
#define INTERRUPT 4
PRIVATE struct errorMsg{
	u_int32 vectorNo ;
	u_int32 errorType;
	u_int8  sign[4]  ;
	u_int8  describe[errorDescribeLength];
};
PRIVATE struct errorMsg exception_msg[20]={0, FAULT,      "#DE","DIVIDED ERROR",
                                          1, TRAP,        "#DB","DEBUG EXCEPTION",
										  2, INTERRUPT,     "-","NOT MASK INTERRPUT",
										  3, TRAP,        "#BP","DEBUG BREAKPOINT",
										  4, TRAP,        "#OF","OVERFLOW",
										  5, FAULT,       "#BR","OVER BOUND",
										  6, FAULT,       "#UD","UNDEFINE OP",
										  7, FAULT,       "#NM","DEVICE NOT AVALIABLE(FLOAT)",
										  8, ABORT,       "#DF","DOUBLE ABORT",
										  9, FAULT,        "# ","COPROCESSOR SEGMENT VOER EXCEPTION",
										  10,FAULT,       "#TS","INVALID TSS",
										  11,FAULT,       "#NP","SEGMENT NOT PRESENT",
										  12,FAULT,       "#SS","STACK FAULT",
										  13,FAULT,       "#GP","GENERAL PROTECTION ERROR",
										  14,FAULT,       "#PF","PAGE FAULT",
										  15,FAULT,        "#-","RESERVED",
										  16,FAULT,       "#MF","X87FPU FLOAT FAULT",
										  17,FAULT,       "#AC","BOUND CHECK",
										  18,ABORT,       "#MC","MACHINE CHECK ABORT",
										  19,FAULT,       "#XF","SIMD FLOAT FAULT"};
PUBLIC void sys_panic(                                            ){
	close_interrupt();
	while(1)
		io_hlt();
}
PUBLIC void sys_exception_panic(u_int32 errorCode,u_int32 vectorNo){
	u_int32 i,j,color=0x00000000;
	u_int32 height=boot_info.screen_height     ;
	u_int32 width =(boot_info.screen_width>>2) ; //用vram32清屏较快 一次4字节
	//清屏
    for(i=0;i<768;i++)
		for(j=0;j<1024;j++)
			*(vram8+(i*1024)+j)=0x2e;
	color=0x3c;
	drawStr("vectorNo" ,0,0  ,0x3c,0x00 );
	drawStr("errorType",0,100,0x3c,0x00 );
	drawStr("sign"     ,0,200,0x3c,0x00 );
	drawStr("errorCode",0,300,0x3c,0x00 );
	drawNum(vectorNo   ,16,0 ,0x1f,0x00 );
	drawNum(exception_msg[vectorNo].errorType,16,100,0x1f,0x00);
	drawStr(exception_msg[vectorNo].sign,16,200,0x1f,0x00     );
	drawNum(errorCode,16,300,0x1f,0x00                        );
	drawStr(exception_msg[vectorNo].describe,32,300,0x1f,0x00 );
	sys_panic();
}