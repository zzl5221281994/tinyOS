#ifndef process_H
#define process_H
#include "F:\work\tolset\tinyOS\kernel\lib\global.h               "
#include "F:\work\tolset\tinyOS\kernel\interrupt\clock.h          "
#define MAX_PROCESS          25
#define MAX_LDT              3
#define MAX_PROCESS_NAME     20

#define STATUS_SUSPEND       1
#define STATUS_EXEC          2
#define STATUS_BLOCK         4

#define INIT_EFLAGS          0x00001202
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
	u_int32 status                     ;
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
extern u_int32 current_exec_pid    ;
extern u_int32 interrupt_mutex     ;
extern void init_tss      (               );
extern int32 createProcess(               );
extern void schedule      (               );
#endif
