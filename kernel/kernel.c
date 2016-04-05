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
#include "bootInfo\bootInfo.h"
#include "lib\zzlOS.h        "
///////////////////////////////////
////////////////////////////////////
extern void io_hlt(void);
extern void write(int i,char color);
static char* zzlOS_str1="\nWed Mar 30 22 50 57 2016\n\nMIT License\nCopyright c 2016 zhuzuolang\n\nPermission is hereby granted free of charge to any person obtaining a copy\nof this software and associated documentation files the Software to deal\nin the Software without restriction including without limitation the rights\nto use copy modify merge publish distribute sublicense and or sell\ncopies of the Software and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions\nThe above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software \nTHE SOFTWARE IS PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY WHETHER IN AN ACTION OF CONTRACT TORT OR OTHERWISE ARISING FROM \nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE\n\nWed Mar 30 22 50 57 2016\n\nMIT License\nCopyright c 2016 zhuzuolang\n\nPermission is hereby granted free of charge to any person obtaining a copy\nof this software and associated documentation files the Software to deal\nin the Software without restriction including without limitation the rights\nto use copy modify merge publish distribute sublicense and or sell\ncopies of the Software and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions\nThe above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software \nTHE SOFTWARE IS PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY WHETHER IN AN ACTION OF CONTRACT TORT OR OTHERWISE ARISING FROM \nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE\n";
static char* errorMessage="init_bootInfo error\n";
void HariMain(void)
{
	init_bootInfo(/*boot_info_p*/);
	drawStr("    vram base",0,0,0x3c,0x00);
	drawNum(boot_info.vram,0,200,0x2e,0x00);
	/*drawStr("screen height",16,0,0x3c,0x00);
	//drawNum(boot_info.screen_height,16,200,0x2e,0x00);
	drawStr(" screen width",32,0,0x3c,0x00);
	//drawNum(boot_info.screen_width,32,200,0x2e,0x00);
	drawStr("    mp length",48,0,0x3c,0x00);
	//drawNum(boot_info.mp_length,48,200,0x2e,0x00);*/
	while(1)
		io_hlt();
}