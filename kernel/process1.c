#include "user_lib\user_lib.h"
struct hd_msg hd;
struct MESSAGE msg_send;
u_int8 buf[512];
void HariMain(void){
	u_int32 pid=get_pid();
	hd.lba=0;
	hd.buf=buf;
	hd.type=HD_IDENTIFY;
		make_msg(&msg_send,pid,1,HD_MSG,BLOCK_NEED);
		msg_send.u.msg_hd=hd;
		send(&msg_send,pid);
		
		int i;
		for(i=0;i<256;i++)
			drawNum(*((u_int16*)buf+i),16*((i*64)/1024),(i*64)%1024);
		draw((u_int8*)((u_int16*)buf+27),0,pid);
		while(1)
		{
			
		};
}