/************************************************************************************
Wed Apr 27 10:19:16 2016

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
#include "F:\work\tolset\tinyOS\kernel\user_lib\user_lib.h        "
#include "F:\work\tolset\tinyOS\kernel\lib\string.h               "
struct MESSAGE msg_send;
struct MESSAGE msg_recv;
struct HD_INFO hd_info;
struct FILE_SYS_INFO fs_info;
u_int16 hd_info_buf[256];
u_int32 fs_pid;
u_int32 identify_hd(                );
u_int32 identify_fs(                );
void HariMain(void){
	fs_pid=get_pid();
	while(identify_hd()==FALSE);
	while(identify_fs()==FALSE);

			struct HD_MSG hd;
	        hd.lba=0;
	        hd.buf=hd_info_buf;
	        hd.type=HD_READ;
			make_msg(&msg_send,fs_pid,HD_DRIVER,HD_MSG_TYPE,BLOCK_NEED);
			msg_send.u.msg_hd=hd;
			u_int32 key=send(&msg_send,fs_pid);
	
	/*struct WD_MSG wnd_msg;
	struct WINDOW wnd;
	wnd.height=300;
	wnd.width=400;
	wnd.backGround=0x26;
	wnd_msg.type=CREATE_WND;
	wnd_msg.wnd=wnd;
	make_msg(&msg_send,fs_pid,3,WD_MSG_TYPE,BLOCK_NEED);
	msg_send.u.msg_wd=wnd_msg;*/
	
	key=send(&msg_send,fs_pid);
	drawNum(1234,0,100);
	while(1);
	
}
u_int32 identify_hd(                                     ){
	struct HD_MSG hd;
	hd.lba=0;
	hd.buf=hd_info_buf;
	hd.type=HD_IDENTIFY;
	make_msg(&msg_send,fs_pid,HD_DRIVER,HD_MSG_TYPE,BLOCK_NEED);
	msg_send.u.msg_hd=hd;
	u_int32 key=send(&msg_send,fs_pid);
	if(key==FALSE)
		return 0;
	else
	{
		memcpy8((u_int8*)(hd_info_buf+10),(u_int8*)hd_info.hd_no,20);//硬盘序列号
		hd_info.hd_no[20]='\0';
		hd_info.hd_type[40]='\0';
		memcpy8((u_int8*)(hd_info_buf+27),(u_int8*)hd_info.hd_type,40);//型号
		hd_info.sectorNum=*((u_int32*)(hd_info_buf+60));
		hd_info.capabilities=*(hd_info_buf+49);
		hd_info.support_cmd=*(hd_info_buf+83);
		return TRUE;
	}
	
}
u_int32 identify_fs(                                     ){
	/*struct HD_MSG hd;
	hd.lba=0;
	hd.buf=hd_info_buf;
	hd.type=HD_READ;
	make_msg(&msg_send,fs_pid,HD_DRIVER,HD_MSG_TYPE,BLOCK_NEED);
	msg_send.u.msg_hd=hd;
	u_int32 key=send(&msg_send,fs_pid);
	if(key==FALSE)
		return FALSE;
	return TRUE;*/
}