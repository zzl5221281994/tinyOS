#include "multiTask\process.h   "
#include "lib\type.h            "
#include "sys.h                 "
struct test_msg test;
u_int8 *vram=0xE0000000;
void make_msg(struct MESSAGE *msg,u_int32 send_pid,u_int32 recv_pid,u_int32 type,u_int32 block){
	msg->send_pid=send_pid;
	msg->recv_pid=recv_pid;
	msg->type=type;
	msg->msg_status=MSG_STATUS_VALID;
	msg->block=block;
	msg->u.msg_test=test;
}
/*void draw(int x,int y,int key){
		int i,j;
		for(i=x;i<x+50;i++)
			for(j=y;j<y+50;j++)
				*(vram+i*1024+j)=key;
}*/
//char*str="abcde";
struct MESSAGE msg_recv;
struct MESSAGE msg_send;
void HariMain(void){
	//msg_recv.msg_status=MSG_STATUS_INVALID;
	int8 key='a';
	int8 l=0;
	while(1)
	{
		int i;
		int8 temp=key+l;
		for(i=0;i<=8;i++)
			test.str[i]=(temp++)%128;
		test.str[9]='\0';
		l++;
		/*l++;
		test.str[0]=('a'+l)%128;
		test.str[1]=('b'+l)%128;
		test.str[2]=('c'+l)%128;
		test.str[3]='\0';
		draw(test.str,0,2);*/
		make_msg(&msg_send,2,3,TEST_MSG,BLOCK_NEED);
		send(&msg_send,2);
		receive(&msg_recv,STATUS_RECV_SPECIFY,3,2);
		awake(msg_recv.send_pid,STATUS_NO_TIME);
		switch(msg_recv.type)
		{
			case TEST_MSG:
		              draw(msg_recv.u.msg_test.str,0,2);
						 break;
			default:
					draw("abcd",0,2);
						 break;
		}
		
		//key+=1;
		//key=key%0x3f;
		
	}
}