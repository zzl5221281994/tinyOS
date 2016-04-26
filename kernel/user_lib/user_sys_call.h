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
extern u_int32 get_pid         (                                                                     );
																										 
#endif
