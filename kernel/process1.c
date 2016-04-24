//#include "multiTask\process.h   "
#include "lib\global.h          "
//#include "sys.h                 "
//struct test_msg test;
//u_int8 *vram=0xE0000000;
/*void make_msg(struct MESSAGE *msg,u_int32 send_pid,u_int32 recv_pid,u_int32 type,u_int32 block){
	msg->send_pid=send_pid;
	msg->recv_pid=recv_pid;
	msg->type=type;
	msg->msg_status=MSG_STATUS_VALID;
	msg->block=block;
	msg->u.msg_test=test;
}*/
/*void draw(int x,int y,int key){
		int i,j;
		for(i=x;i<x+50;i++)
			for(j=y;j<y+50;j++)
				*(vram+i*1024+j)=key;
}*/
char*str="abcde";
void HariMain(void){
	//struct MESSAGE msg_recv;
	//struct MESSAGE msg_send;
	//msg_recv.msg_status=MSG_STATUS_INVALID;
	//test.str="process1";
	//u_int32 key=0;
	int l;
	while(1)
	{
		u_int8 *vram=0xE0000000;
		*(vram+l+200*1024)=0x3c;
		int i,j,key=0;
	//for(i=0;i<10;i++)
		for(j=0;j<10000;j++)
			   key+=i+j;
		l++;
		//make_msg(&msg_send,2,3,TEST_MSG,BLOCK_NEED);
		//send(&msg_send,2);
		
		//receive(&msg_recv,STATUS_RECV_ANY,0,2);
		//switch(msg.type)
		//{
		//	case TEST_MSG:
		//	             draw(0,0,0x3c);
		//				 break;
		//	default:
		//				draw(0,100,key);
		//				 break;
		//}
		
		//key+=1;
		//key=key%0x3f;
		
	}
}