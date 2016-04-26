#include "user_lib.h"
PUBLIC void make_msg(struct MESSAGE *msg,u_int32 send_pid,u_int32 recv_pid,u_int32 type,u_int32 block){
	msg->send_pid=send_pid;
	msg->recv_pid=recv_pid;
	msg->type=type;
	msg->msg_status=MSG_STATUS_VALID;
	msg->block=block;
}