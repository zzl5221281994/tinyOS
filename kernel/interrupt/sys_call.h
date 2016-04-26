/************************************************************************************
Tue Apr 19 10:32:53 2016

MIT License
Copyright (c) 2016 zhuzuolang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************************/
#ifndef sys_call_H
#define sys_call_H
#include "F:\work\tolset\tinyOS\kernel\graphics\font.h          "
#include "F:\work\tolset\tinyOS\kernel\multiTask\process.h      "
#include "F:\work\tolset\tinyOS\kernel\debug\debug.h            "
#define SYS_CALL_NUM    9
extern u_int32 sys_call_table[SYS_CALL_NUM];
extern u_int32 msg_queue_table[MAX_PROCESS];
extern void sys_call            (                                                                );
extern void init_msg_queue      (                                                                );
extern void init_sys_call_table (                                                                );
extern u_int32 l_addr2liner_addr(u_int32 addr,u_int32 pid,u_int32 type                           );

extern u_int32 send_msg         (struct MESSAGE*msg, u_int32 call_pid                            );
extern u_int32 recv_msg         (struct MESSAGE*msg,u_int32 type,u_int32 specify,u_int32 call_pid);
#endif
