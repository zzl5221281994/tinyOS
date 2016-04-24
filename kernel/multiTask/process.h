#ifndef process_H
#define process_H
#include "F:\work\tolset\tinyOS\kernel\lib\global.h               "
#include "F:\work\tolset\tinyOS\kernel\interrupt\clock.h          "
#define MAX_PROCESS          25
#define MAX_LDT              3
#define MAX_PROCESS_NAME     20
#define INIT_EFLAGS          0x00001202
/*����״̬*/
#define STATUS_RECV_ANY      1/*����״̬������*/
#define STATUS_RECV_SPECIFY  2/*����״̬������*/
#define STATUS_RECV_INT      4/*����״̬������*/
#define STATUS_SEND_PENDING  8/*����״̬������*/
#define STATUS_RECV_PENDING  16/*����״̬������*/
#define STATUS_NO_TIME       32/*����״̬,������*/
#define STATUS_EXEC          64/*����״̬*/


//Ӳ����Ϣ�ṹ
#define HD_READ           1
#define HD_WRITE          2
#define HD_IDENTIFY       4
struct hd_msg{
	u_int32 lba       ;
	void*   buf       ;
	u_int32 type      ;	
};
//������Ϣ�ṹ
struct wd_msg{
	u_int32 type   ; 
	void* liner_buf;
};
//�ж���Ϣ�ṹ
struct int_msg{
	u_int32 intNo; 
};
struct test_msg{
	char str[10];
};
//��Ϣ����
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
	u_int32 block     ;/*�����̷�������Ϣ�󣬸��ݴ��ֶ��ж��Ƿ���Ҫ�������ͽ���
	                     �����̴�������Ϣ�󣬸��ݴ��ֶ��ж��Ƿ���Ҫ���ѷ��ͽ���*/
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
	struct stack_frame frame           ;//����������
	u_int32 ldtSelector                ;//ldtѡ����
	u_int32 pid                        ;//����id
	u_int32 max_time                   ;//����ʱ��Ƭ
	u_int32 current_time               ;
	u_int32 priority                   ;//�������ȼ�
	
	/*���Ե�ַ*******/
	u_int32 codeBase                   ;
	u_int32 dataBase                   ;
	u_int32 stackBase                  ;
	/*���̼�ͨ�����*/
	u_int32 status                     ;/*���ΪSTATUS_RECV_SPECIFY ,specifyΪ�ȴ�pid.
	                                      ���ΪSTATUS_RECV_INT��specifyΪ�жϺ�.
										  ���Ϊ��������specify��Ч*/
	u_int32 specify                    ;
	struct MESSAGE*msg_queue           ;/*ѭ����Ϣ�������*/
	struct MESSAGE*msg                 ;
	u_int32 msg_queue_size             ;/**/
	u_int8  ldtDescriptor[MAX_LDT][8]  ;//ldt������   code-data-stack
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
	u_int16	iobase  ;//I/Oλͼ��ַ���ڻ����TSS�ν��ޣ��ͱ�ʾû��I/O���λͼ 
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
/*�ֲ���ַת��Ϊȫ�����Ե�ַ*/
extern u_int32 l_addr2liner_addr(u_int32 addr,u_int32 pid,u_int32 type);/*type=0,��ʾ����Σ�
                                                                       type=1,��ʾ���ݶΣ�
																	   type=2,��ʾ��ջ��*/
#endif
