/************************************************************************************
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
#include "graphics\font.h"
#include "lib\zzlOS.h    "
extern void io_hlt(void);
extern void write(int i,char color);
static char* zzlOS_str1="0123456789A";
static char* zzlOS_str2="qwertyuiopasdfghjklzxcvbnm";
static char* zzlOS_str3="QWERTYUIOPASDFGHJKLZXCVBNM";
void HariMain(void)
{
	init_font();
	drawStr(zzlOS_str1,0,0,0x01,0x00);
	drawStr(zzlOS_str2,16,0,0x01,0x00);
	drawStr(zzlOS_str3,32,0,0x01,0x00);
	while(1)
		io_hlt();
}