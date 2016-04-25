//test
extern u_int32 send(struct MESSAGE*msg,u_int32 call_pid                                );
extern u_int32 receive(struct MESSAGE*msg,u_int32 type,u_int32 specify,u_int32 call_pid);

extern u_int32 draw(u_int8 *str ,u_int32 key,u_int32 pid                               );
extern void sleep  (u_int32 pid,u_int32 status                                         );
extern void awake  (u_int32 pid,u_int32 parm                                           );
//test