/************************************************************************************
Sat Apr 30 11:41:26 2016

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
#ifndef window_H
#define window_H
#include "F:\work\tolset\tinyOS\kernel\user_lib\user_lib.h        "
#define KEYBOARD   1
#define MOUSE      12
struct MESSAGE msg_send;
struct MESSAGE msg_recv;
struct screen_info{
	u_int8* vram;
	u_int32 screen_width;
	u_int32 screen_height;
};
struct buffer_info{
	u_int8*keyBoardBuffer;
	u_int8*mouseBuffer;
};
extern void mouse_proc   (u_int8*buffer,u_int32 pos          );
extern void keyBoard_proc(u_int8*buffer,u_int32 pos          );
extern void init_screen  (                                   );
extern void createWindow (struct WINDOW *wnd,u_int32 send_pid);
extern void updateFrame  (struct WINDOW *wnd,u_int32 send_pid);
#endif