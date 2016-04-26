#ifndef user_lib_H
#define user_lib_H
#include "F:\work\tolset\tinyOS\kernel\lib\global.h             "
#include "F:\work\tolset\tinyOS\kernel\multiTask\process.h      "
#include "F:\work\tolset\tinyOS\kernel\multiTask\message.h      "
#include "user_sys_call.h                                       "
extern void make_msg(struct MESSAGE *msg,u_int32 send_pid,u_int32 recv_pid,u_int32 type,u_int32 block);
#endif
