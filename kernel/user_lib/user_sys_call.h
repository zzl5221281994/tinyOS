/************************************************************************************
Wed Apr 27 10:17:58 2016

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
#ifndef user_sys_call_H
#define user_sys_call_H
#include "F:\work\tolset\tinyOS\kernel\lib\global.h             "
#include "F:\work\tolset\tinyOS\kernel\multiTask\message.h      "
extern void assertion_failure(char *exp, char *file, char *base_file, int line                       );
#define assert(exp)  if (exp);else assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)
extern u_int8  io_in8                (u_int32 port                             );
extern u_int16 io_in16               (u_int32 port                             );
extern u_int32 io_in32               (u_int32 port                             );
extern void    io_out8               (u_int32 port,u_int32 data                );
extern void    io_out16              (u_int32 port,u_int32 data                );
extern void    io_out32              (u_int32 port,u_int32 data                );


extern u_int32 send   (struct MESSAGE*msg,u_int32 call_pid                                              );
extern u_int32 receive(struct MESSAGE*msg,u_int32 type,u_int32 specify,u_int32 call_pid                 );

extern void sleep     (u_int32 pid,u_int32 status                                                       );
extern void awake     (u_int32 pid,u_int32 parm                                                         );

extern u_int32 draw   (u_int8 *str ,u_int32 key,u_int32 pid                                             );
extern u_int32 drawNum(u_int32 num,u_int32 x,u_int32 y                                                  );
extern u_int32 transTolinerAddr(u_int32 addr,u_int32 pid,u_int32 type                                   );/*type=0,表示代码段，
                                                                                                       type=1,表示数据段，
																	                                     type=2,表示堆栈段*/
extern u_int32 get_pid         (                                                                        );
extern void get_screen_info    ( u_int32 addr,u_int32 pid                                               );
																										 
#endif
