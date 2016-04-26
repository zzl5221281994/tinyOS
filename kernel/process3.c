#include "user_lib\user_lib.h"
struct test_msg test;
struct MESSAGE msg_recv;
struct MESSAGE msg_send;
void HariMain(void){
	int8 key='a';
	int8 l=0;
	u_int32 pid=get_pid();
	while(1)
	{
		u_int32 key=draw("qwerty",0,pid);
		if(key!=61)
			draw("qqqqqq",1,pid);
	}
}