#include "F:\work\tolset\tinyOS\kernel\user_lib\user_lib.h        "
#include "F:\work\tolset\tinyOS\kernel\lib\string.h               "
struct MESSAGE msg_send;
struct MESSAGE msg_recv;
struct screen_info{
	u_int8* vram;
	u_int32 screen_width;
	u_int32 screen_height;
};
struct screen_info screen;
u_int32 wd_pid;
u_int8* vram;
u_int8 backGround=0x1c;
void init_screen (u_int8 byte1,u_int8 byte2,u_int8 byte3);
void createWindow(struct WINDOW *wnd,u_int32 send_pid);
void updateFrame (struct WINDOW *wnd,u_int32 send_pid);
//test
u_int32 key=0;
//test
void HariMain(void){
	wd_pid=get_pid();
	get_screen_info(&screen,wd_pid);
	vram=screen.vram;
	//test
	/*struct FILE_MSG file_msg;
	file_msg.type=FILE_OPEN;
	int8*name="haribote.sys";
	file_msg.file_name=name;
	make_msg(&msg_send,wd_pid,2,FILE_MSG_TYPE,BLOCK_NEED);
	msg_send.u.msg_file=file_msg;
	u_int32 handle=send(&msg_send,wd_pid);
	drawNum(handle,500,500);
	
	
	
	
	struct I_NODE inode;
	file_msg.type=FILE_IDENTIFY;
	file_msg.handle=6;
	file_msg.inode=&inode;
	make_msg(&msg_send,wd_pid,2,FILE_MSG_TYPE,BLOCK_NEED);
	msg_send.u.msg_file=file_msg;
	send(&msg_send,wd_pid);
	
	draw(inode.file_name,0,wd_pid);
	drawNum(inode.byte_size,100,0);
	drawNum(inode.startSector,100,100);
	drawNum(inode.sectorNum,100,200);
	drawNum(inode.status,100,300);
	
	
	file_msg.type=POWER_OFF;
	file_msg.handle=handle;
	msg_send.u.msg_file=file_msg;
	send(&msg_send,wd_pid);
	file_msg.type=POWER_OFF;
	file_msg.handle=handle;
	msg_send.u.msg_file=file_msg;
	send(&msg_send,wd_pid);*/
	//test
	while(1)
	{
		receive(&msg_recv,STATUS_RECV_SPECIFY,2,wd_pid);
		u_int32 msg_type=msg_recv.type;
		u_int32 send_pid=msg_recv.send_pid;
		switch(msg_type)
		{
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
void init_screen(u_int8 byte1,u_int8 byte2,u_int8 byte3){
	int i,j;
	//for(i=0;i<screen.screen_height;i++)
	//	for(j=0;j<screen.screen_width;j+=3)
	for(j=0;j<1024*20*3;j+=3)
		{
			*(vram+j)=byte1;
			*(vram+j+1)=byte2;
			*(vram+j+2)=byte3;
		}
}
void createWindow(struct WINDOW *wnd,u_int32 send_pid){
	int i,j;
	u_int32 h=100+wnd->height;
	u_int32 w=100+wnd->width;
	for(i=100;i<h;i++)
		for(j=100;j<w;j++)
			*(vram+i*1024+j)=wnd->backGround;
	return;
}
void updateFrame (struct WINDOW *wnd,u_int32 send_pid){
	return;
}