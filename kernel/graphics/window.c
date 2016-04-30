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
#include "F:\work\tolset\tinyOS\kernel\lib\string.h               "
#include "F:\work\tolset\tinyOS\kernel\user_lib\user_lib.h        "
#include "window.h                                                "
struct MESSAGE msg_send;
struct MESSAGE msg_recv;

struct screen_info screen;
struct buffer_info buffer;
u_int32 wd_pid;
u_int8* vram8;
#define LENX 16
#define LENY 16
static char cursor[16][16] = {
        "**************..",
		"*OOOOOOOOOOO*...",
		"*OOOOOOOOOO*....",
		"*OOOOOOOOO*.....",
		"*OOOOOOOO*......",
		"*OOOOOOO*.......",
		"*OOOOOOO*.......",
		"*OOOOOOOO*......",
		"*OOOO**OOO*.....",
		"*OOO*..*OOO*....",
		"*OO*....*OOO*...",
		"*O*......*OOO*..",
		"**........*OOO*.",
		"*..........*OOO*",
		"............*OO*",
		".............***"
	};
void HariMain(void){
	wd_pid=get_pid();
	get_screen_info((u_int32)&screen,wd_pid);
	get_buffer_info((u_int32)&buffer,wd_pid);
	vram8=screen.vram;
    //while(1);
	while(1)
	{
		receive(&msg_recv,STATUS_RECV_ANY,0,wd_pid);
		u_int32 msg_type=msg_recv.type;
		u_int32 send_pid=msg_recv.send_pid;
		switch(msg_type)
		{
			case INT_MSG_TYPE:
			{
				struct INT_MSG*msg=&msg_recv.u.msg_int;
				u_int32 intNo=msg->intNo;
				u_int32 status=msg->status;/*实际为mouse_read_pos**/
				switch(intNo)
				{
					case MOUSE:
                              //drawNum(status,100,100);
					          mouse_proc(buffer.mouseBuffer,status);
							break;
					case KEYBOARD:
					        //keyBoard_proc();
							break;
					default:
					        break;
				}
				break;
			}
			case WD_MSG_TYPE:
			{
				struct WD_MSG*wnd_msg=&(msg_recv.u.msg_wd);
			    switch(wnd_msg->type)
				{
					case CREATE_WND:
			                createWindow(&wnd_msg->wnd,send_pid );
							awake(send_pid,TRUE);
							break;
					case UPDATE_FRAME:
		                    updateFrame (&wnd_msg->frame,send_pid);
							awake(send_pid,TRUE);
							break;
					default:
							awake(send_pid,FALSE);
							break;
				}
				break;
			}
			default:
			                awake(send_pid,FALSE);
							break;
		}
	};
}
//test
void drawMouse(int32 x,int32 y){
	int i,j;
	for(i=x;i<x+LENX;i++)
		for(j=y;j<y+LENY;j++)
		{
			if(cursor[i-x][j-y]=='*')
				*(vram8+1024*i+j)=0x1f;
			if(cursor[i-x][j-y]=='.')
				*(vram8+1024*i+j)=0xff;
			if(cursor[i-x][j-y]=='O')
				*(vram8+1024*i+j)=0x3c;
		}
}
//test
int32 startX=500,startY=500;
void mouse_proc   (u_int8*buffer,u_int32 pos          ){
	u_int8 byte1=*(buffer+pos+2);
	  int8     y=*(buffer+pos+1);
	  int8     x=*(buffer+pos+0);
      //if((byte1&0x80)||(byte1&0x40))
	//	  return;
	 update();
	 	 drawNum(startX+(-x),0,0);
	 drawNum(startY+y,0,100);
	 
	 startX=startX+(-x);
	 startY=startY+y;
	 if(startX<0)
		 startX=0;
	 else if(startX>768)
		 startX=767;
	 if(startY<0)
		 startY=0;
	 else if(startY>=1024)
		 startY=1023;
	 

	 drawMouse(startX,startY);
}
void update(){
	int i,j;
	for(i=startX;i<startX+LENX;i++)
		for(j=startY;j<startY+LENY;j++)
				*(vram8+1024*i+j)=0xff;
}
//test
void keyBoard_proc(u_int8*buffer,u_int32 pos          ){
	
}
void init_screen(){
	
			
}
void createWindow(struct WINDOW *wnd,u_int32 send_pid){
	int i,j;
	u_int32 h=100+wnd->height;
	u_int32 w=100+wnd->width;
	for(i=100;i<h;i++)
		for(j=100;j<w;j++)
			*(vram8+i*1024+j)=wnd->backGround;
	return;
}
void updateFrame (struct WINDOW *wnd,u_int32 send_pid){
	return;
}