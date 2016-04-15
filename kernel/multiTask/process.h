#ifndef process_H
#define process_H
#include "F:\work\tolset\tinyOS\kernel\lib\global.h          "
#define MAX_PROCESS          25
#define MAX_LDT              3
#define MAX_PROCESS_NAME     20

#define STATUS_SUSPEND       1
#define STATUS_EXEC          2
#define STATUS_BLOCK         4
struct stack_frame{
	u_int32 gs        ;
	u_int32 fs        ;
	u_int32 es        ;
	u_int32 ds        ;
	
	u_int32 edi       ;
	u_int32 esi       ;
	u_int32 ebp       ;
	u_int32 kernel_esp;
	
	u_int32 ebx       ;
	u_int32 edx       ;
	u_int32 ecx       ;
	u_int32 eax       ;
	u_int32 retaddr   ;
	u_int32 eip       ;
	u_int32 cs        ;
	u_int32 eflags    ;
	u_int32 esp       ;
	u_int32 ss        ;
};
struct proc_table{
	struct stack_frame frame           ;//����������
	u_int32 ldtSelector                ;//ldtѡ����
	u_int32 pid                        ;//����id
	
	u_int32 priority                   ;//�������ȼ�
	u_int32 status                     ;
	u_int8  ldtDescriptor[MAX_LDT][8]  ;//ldt������
	int8    p_name[MAX_PROCESS_NAME]   ;//������
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
	u_int32	flags   ;
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
	u_int16	iobase  ;//I/Oλͼ��ַ���ڻ����TSS�ν��ޣ��ͱ�ʾû��I/O����λͼ 
};
extern void init_tss();
#endif