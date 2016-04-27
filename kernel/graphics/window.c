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
u_int8 backGround=0x2e;
void init_screen (                                 );
void createWindow(struct WINDOW *wnd,u_int32 send_pid);
void updateFrame (struct WINDOW *wnd,u_int32 send_pid);
void HariMain(void){
	drawNum(1234,100,100);
	wd_pid=get_pid();
	get_screen_info(&screen,wd_pid);
	vram=screen.vram;
	init_screen();
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
void init_screen(                                 ){
	int i,j;
	for(i=0;i<screen.screen_height;i++)
		for(j=0;j<screen.screen_width;j++)
			*(vram+i*screen.screen_width+j)=backGround;
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