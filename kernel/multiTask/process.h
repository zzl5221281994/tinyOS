#ifndef process_H
#define process_H
#include "F:\work\tolset\tinyOS\kernel\lib\global.h               "
#include "F:\work\tolset\tinyOS\kernel\interrupt\clock.h          "
#define MAX_PROCESS          25
#define MAX_LDT              3
#define MAX_PROCESS_NAME     20
#define INIT_EFLAGS          0x00001202
/*进程状态*/
#define STATUS_RECV_ANY      1/*挂起状态，阻塞*/
#define STATUS_RECV_SPECIFY  2/*挂起状态，阻塞*/
#define STATUS_RECV_INT      4/*挂起状态，阻塞*/
#define STATUS_SEND_PENDING  8/*挂起状态，阻塞*/
#define STATUS_RECV_PENDING  16/*挂起状态，阻塞*/
#define STATUS_NO_TIME       32/*挂起状态,可运行*/
#define STATUS_EXEC          64/*运行状态*/


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
	void* liner_buf;
};
//中断消息结构
struct int_msg{
	u_int32 intNo; 
};
struct test_msg{
	char str[10];
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
struct stack_frame{
	u_int32 gs        ;
	u_int32 fs        ;
	u_int32 es        ;
	u_int32 ds        ;
	
	u_int32 ss		  ;   //<----EAX    INTERRUPT---->PUSHAD
	u_int32 esp  	  ;
	u_int32 eflags	  ;
	u_int32 cs		  ;
	u_int32 eip		  ;
	u_int32 eax		  ;
	u_int32 ecx		  ;
	u_int32 edx		  ;
	u_int32 ebx		  ;
	u_int32 esp2	  ;
	u_int32 ebp		  ;
	u_int32 esi		  ;
	u_int32 edi		  ;
};
struct proc_table{
	struct stack_frame frame           ;//进程上下文
	u_int32 ldtSelector                ;//ldt选择子
	u_int32 pid                        ;//进程id
	u_int32 max_time                   ;//进程时间片
	u_int32 current_time               ;
	u_int32 priority                   ;//进程优先级
	
	/*线性地址*******/
	u_int32 codeBase                   ;
	u_int32 dataBase                   ;
	u_int32 stackBase                  ;
	/*进程间通信相关*/
	u_int32 status                     ;/*如果为STATUS_RECV_SPECIFY ,specify为等待pid.
	                                      如果为STATUS_RECV_INT，specify为中断号.
										  如果为其他，则specify无效*/
	u_int32 specify                    ;
	struct MESSAGE*msg_queue           ;/*循环消息缓冲队列*/
	struct MESSAGE*msg                 ;
	u_int32 msg_queue_size             ;/**/
	u_int8  ldtDescriptor[MAX_LDT][8]  ;//ldt描述符   code-data-stack
	int8    p_name[MAX_PROCESS_NAME]   ;//进程名
};
struct TSS{
	u_int32	backlink;
	u_int32	esp0    ;	
	u_int32	ss0     ;	
	u_int32	esp1    ;
	u_int32	ss1     ;
	u_int32	esp2    ;
	u_int32	ss2     ;
	u_int32	cr3     ;
	u_int32	eip     ;
	u_int32	eflags  ;
	u_int32	eax     ;
	u_int32	ecx     ;
	u_int32	edx     ;
	u_int32	ebx     ;
	u_int32	esp     ;
	u_int32	ebp     ;
	u_int32	esi     ;
	u_int32	edi     ;
	u_int32	es      ;
	u_int32	cs      ;
	u_int32	ss      ;
	u_int32	ds      ;
	u_int32	fs      ; 
	u_int32	gs      ;
	u_int32	ldt     ; 
	u_int16	trap    ;
	u_int16	iobase  ;//I/O位图基址大于或等于TSS段界限，就表示没有I/O许可位图 
};


extern u_int32 current_exec_pid                    ;
extern u_int32 interrupt_mutex                     ;
extern struct proc_table process_table[MAX_PROCESS];

extern void init_tss         (                                    );
extern void schedule         (                                    );
extern int32 createServer    (u_int32 offset,u_int32 max_time     );
extern int32 createProcess   (                                    );
extern void sleep            (u_int32 pid,u_int32 status          );
extern void awake            (u_int32 pid,u_int32 parm            );
/*局部地址转换为全局线性地址*/
extern u_int32 l_addr2liner_addr(u_int32 addr,u_int32 pid,u_int32 type);/*type=0,表示代码段，
                                                                       type=1,表示数据段，
																	   type=2,表示堆栈段*/
#endif
