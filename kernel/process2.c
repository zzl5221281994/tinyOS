#include "user_lib\user_lib.h"
struct test_msg test;
struct MESSAGE msg_recv;
struct MESSAGE msg_send;
void HariMain(void){
	int8 key='q';
	int8 l=0;
	while(1)
	{
		int i;
		int8 temp=key+l;
		for(i=0;i<=8;i++)
			test.str[i]=(temp++)%128;
		test.str[9]='\0';
		l++;
		make_msg(&msg_send,2,1,TEST_MSG,BLOCK_NEED);
		msg_send.u.msg_test=test;
		receive(&msg_recv,STATUS_RECV_SPECIFY,1,2);
		awake(msg_recv.send_pid,STATUS_NO_TIME);
		send(&msg_send,2);
		switch(msg_recv.type)
		{
			case TEST_MSG:
		             draw(msg_recv.u.msg_test.str,1,2);
					 break;
			default:
						draw("qqqqqqqqq",1,2);
						 break;
		}	
	}
}