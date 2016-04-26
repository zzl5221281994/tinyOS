#ifndef message_H
#define message_H
#include "F:\work\tolset\tinyOS\kernel\lib\global.h                 "
//硬盘消息结构
#define HD_READ           1
#define HD_WRITE          2
#define HD_IDENTIFY       4
struct hd_msg{
	u_int32 lba       ;
	void*   buf       ;
	u_int32 type      ;	
};
//窗口消息结构
struct wd_msg{
	u_int32 type   ; 
	void*   buf    ;  
};
//中断消息结构
struct int_msg{
	u_int32 intNo;
    u_int32 status;	
};
struct test_msg{
	int8 str[10];
};
//消息类型
#define HD_MSG   1
#define WD_MSG   2 
#define INT_MSG  3
#define TEST_MSG 4

#define MSG_STATUS_VALID     1
#define MSG_STATUS_INVALID   0

#define BLOCK_NEED           1
#define BLOCK_NOT_NEED       0
struct MESSAGE{
	u_int32 send_pid  ;
	u_int32 recv_pid  ;
	u_int32 msg_status;
	u_int32 type      ;
	u_int32 block     ;/*当进程发送完消息后，根据此字段判断是否需要阻塞发送进程
	                     当进程处理完消息后，根据此字段判断是否需要唤醒发送进程*/
	union {
		struct hd_msg   msg_hd;
		struct wd_msg   msg_wd;
		struct int_msg msg_int;
		struct test_msg msg_test;
	} u;
};
extern void make_msg(struct MESSAGE *msg,u_int32 send_pid,u_int32 recv_pid,u_int32 type,u_int32 block);
#endif
