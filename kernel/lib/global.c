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
#include "global.h"
/*
%macro Descriptor 3
		dw %2 & 0FFFFh
		dw %1 & 0FFFFh
		db (%1>>16) & 0FFh
		dw ((%2>>8) & 0f00h) | (%3 & 0f0ffh)
		db (%1>>24) & 0ffh
%endmacro
*/
PUBLIC void gen_normalDescriptor(u_int8 desc[8],u_int32 base,u_int32 limit,u_int32 attribute                       ){
	u_int16 bit1=limit&0xffff;
	desc[0]= bit1&0x00ff;
	desc[1]=(bit1&0xff00)>>8;
	u_int16 bit2= base&0xffff;
	desc[2]= bit2&0x00ff;
	desc[3]=(bit2&0xff00)>>8;
	u_int8  bit3=(base>>16)&0x00ff;
	desc[4]=bit3;
	u_int16 bit4=((limit>>8)&0xf00)|(attribute&0xf0ff);
	desc[5]=bit4&0x00ff;
	desc[6]=(bit4&0xff00)>>8;
	u_int8  bit5=(base>>24)&0xff;
	desc[7]=bit5;
	
}
PUBLIC void   gen_gateDescriptor(u_int8 desc[8],u_int32 Selector,u_int32 offset,u_int32 DCount,u_int32 attribute){
	desc[0]=offset&0x000000ff;
	desc[1]=(offset&0x0000ff00)>>8;
	desc[2]=Selector&0x000000ff;
	desc[3]=(Selector&0x0000ff00)>>8;
	desc[4]=DCount&0x0000001f;
	desc[5]=attribute&0x000000ff;
	desc[6]=(offset&0x00ff0000)>>16;
	desc[7]=(offset&0xff000000)>>24;
	
}