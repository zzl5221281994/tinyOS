#include "process.h"
#include "F:\work\tolset\tinyOS\kernel\bootInfo\bootInfo.h          "
#include "F:\work\tolset\tinyOS\kernel\lib\string.h                 "
#include "F:\work\tolset\tinyOS\kernel\interrupt\interrupt.h        "
#include "F:\work\tolset\tinyOS\kernel\debug\debug.h                "
#include "F:\work\tolset\tinyOS\kernel\kernelFun.h                  "
//;|1MB~~~~~~~~~~~5MB|   |5MB~~~~~~~~~~~~~~~~~~~~~~10MB|    |10MB~~~~~~~~~~~~~~~~~~~~~~20MB|  |20MB~~~~~~~~~~~~~|
//;    |内核代码|           |内核数据---> <---内核栈|                系统服务程序区              用户程序区
PUBLIC struct proc_table process_table[MAX_PROCESS];
PUBLIC struct TSS global_tss                       ;
PUBLIC u_int32 current_exec_pid=0                  ;
PRIVATE u_int32 server_base  = 0xA00000            ;//10MB
PRIVATE u_int32 user_base    =0x1400000            ;//20MB
PRIVATE u_int32 allo_len=0xFFFF                    ;//64KB
PRIVATE u_int32 allo_len_4KB=32                    ;//128KB

PUBLIC u_int32 schedule_num=0                      ;
PUBLIC u_int32 global_clock=0                      ;

PUBLIC void init_tss          (                                                           ){
	/*全局只有一个TSS*/
	u_int32 tss_sel=addDes(0,0xfffff,SegDesc_Property_32 |SegDesc_Property_4KB|SegDesc_Property_RW);
	global_tss.ss0=tss_sel*8;
	global_tss.esp0=topOfstack_kernel;
	global_tss.iobase=104;
	u_int32 sel=addDes((u_int32)(&global_tss),103,SegDesc_Property_386TSS);
	process_table[0].current_time=1;
	process_table[0].max_time=1;
	loadTss(sel*8);
}
PRIVATE u_int32 get_valid_pid (                                                           ){
	int i;
	for(i=1;i<MAX_PROCESS;i++)
		if(process_table[i].status==STATUS_INVALID)
			return i;
	return FALSE;
}
PUBLIC void schedule          (                                                           ){
	//test
	kernel_mutex++;
	schedule_num++;
	//drawNum(current_exec_pid,(16*((schedule_num*64)/1024))%768,(schedule_num*64)%1024,0x3c,0x00);
	/*进程调度*/
	process_table[current_exec_pid].current_time--;
	
	if(process_table[current_exec_pid].current_time==0||(process_table[current_exec_pid].status!=STATUS_NO_TIME))
	{
		process_table[current_exec_pid].current_time=process_table[current_exec_pid].max_time;
		current_exec_pid=(current_exec_pid+1)%MAX_PROCESS;
		while(process_table[current_exec_pid].status!=STATUS_NO_TIME||current_exec_pid==0)
			current_exec_pid=(current_exec_pid+1)%MAX_PROCESS;
	}
	process_table[current_exec_pid].status=STATUS_EXEC;
	/*进程切换*/
	loadLdt((process_table[current_exec_pid].ldtSelector)*8);
    
	u_int32 eax=process_table[current_exec_pid].frame.eax;
	u_int32 ecx=process_table[current_exec_pid].frame.ecx;
	u_int32 edx=process_table[current_exec_pid].frame.edx;
	u_int32 ebx=process_table[current_exec_pid].frame.ebx;
	u_int32 ebp=process_table[current_exec_pid].frame.ebp;
	u_int32 esi=process_table[current_exec_pid].frame.esi;
	u_int32 edi=process_table[current_exec_pid].frame.edi;
	
	u_int32 eip=process_table[current_exec_pid].frame.eip      ;
	u_int32 cs=process_table[current_exec_pid].frame.cs        ;
	u_int32 eflags=process_table[current_exec_pid].frame.eflags;
	u_int32 esp=process_table[current_exec_pid].frame.esp      ;
	u_int32 ss=process_table[current_exec_pid].frame.ss        ;
	u_int32 ds=process_table[current_exec_pid].frame.ds        ;
	loadReg(eip,cs,eflags,esp,ss,eax,ecx,edx,ebx,ebp,esi,edi,ds);
}
PUBLIC int32 createServer     (u_int8*hrb_buf,u_int32 buf_len,u_int32 max_time,u_int32 DPL){
	/*系统服务程序结构 .hrb为通用的可执行文件*/
	//┏━━━━━━━━━━┓
	//┃0	                ┃
	//┃代码段  基地址为    ┃
	//┃64KB   server_base  ┃
	//┃                    ┃
	//┃64KB                ┃
	//┃堆栈段  基地址为0   ┃
	//┃128KB<----ESP的值   ┃
	//┃                    ┃
	//┃128KB<----HRB映像的	┃
    //┃        数据起始地址┃
	//┃数据段  基地址为0   ┃
	//┃192KB               ┃
	//┃                    ┃
	//┃                    ┃
	//┃                    ┃
	//┃                    ┃
	//┗━━━━━━━━━━┛
	u_int8 desc[8];
	u_int32 valid_pid=get_valid_pid();
	assert(valid_pid!=FALSE);
	process_table[valid_pid].max_time=max_time;
	process_table[valid_pid].current_time=max_time;
	
	u_int32 data_len  =*(u_int32*)(hrb_buf+16);//得到进程的实际数据段大小
	u_int32 data_pos  =*(u_int32*)(hrb_buf+20);//数据在可执行文件中的偏移
	u_int32 data_start=*(u_int32*)(hrb_buf+12);//得到数据在数据段中的开始位置
	//建立LDT代码段
	gen_normalDescriptor(desc,server_base,allo_len,SegDesc_Property_EXEC_R|SegDesc_Property_32|SegDesc_Property_DPL1);
	memcpy8(desc,process_table[valid_pid].ldtDescriptor[0],8);
	memcpy8((u_int8*)hrb_buf,(u_int8*)server_base,buf_len);
	process_table[valid_pid].codeBase=server_base;
	server_base+=allo_len+1;
	
	//建立LDT堆栈段
	//系统服务的堆栈段基地址都为0，与内核共享一个段
	gen_normalDescriptor(desc,0,0xfffff,SegDesc_Property_4KB|SegDesc_Property_RW|SegDesc_Property_32|SegDesc_Property_DPL1);
	memcpy8(desc,process_table[valid_pid].ldtDescriptor[1],8);
	process_table[valid_pid].stackBase=0;
	/*此处由于ss寄存器要与ds寄存器一致，因此DS和SS是全局4G段，但ESP的初始值为分配的64KB栈 栈顶*/
	
	//建立LDT数据段
	//系统服务的数据段基地址都为0，与内核共享一个段
	//注意，此时data_start==server_base
	gen_normalDescriptor(desc,0,0xfffff,SegDesc_Property_4KB|SegDesc_Property_RW|SegDesc_Property_32|SegDesc_Property_DPL1);
	memcpy8(desc,process_table[valid_pid].ldtDescriptor[2],8);
	memcpy8((u_int8*)(hrb_buf+data_pos),(u_int8*)(0+data_start),data_len);
	process_table[valid_pid].dataBase=0;
	server_base+=2*allo_len+2;
	
	/*初始化进程初始信息*/
	process_table[valid_pid].frame.esp   =server_base-1;
	process_table[valid_pid].frame.eip   =0x001b;
	process_table[valid_pid].frame.eflags=INIT_EFLAGS;
	process_table[valid_pid].frame.ss    =1*8+Selector_TI_Ldt+Selector_RPL1;
	process_table[valid_pid].frame.cs    =0*8+Selector_TI_Ldt+Selector_RPL1;
	process_table[valid_pid].frame.ds    =2*8+Selector_TI_Ldt+Selector_RPL1;
	process_table[valid_pid].pid         =valid_pid;
	process_table[valid_pid].status      =STATUS_NO_TIME;
	
	//
	process_table[valid_pid].dataBase=0                  ;
	process_table[valid_pid].codeBase=boot_info.codeBase ;
	process_table[valid_pid].stackBase=0                 ;
	//ldtDescriptor
	process_table[valid_pid].ldtSelector=addDes((u_int32)process_table[valid_pid].ldtDescriptor,3*8-1,SegDesc_Property_LDT);
	return valid_pid;
}
PUBLIC int32 createProcess    (u_int8*hrb_buf,u_int32 buf_len,u_int32 max_time,u_int32 DPL){
	/*用户程序结构 .hrb为通用的可执行文件*/
	//┏━━━━━━━━━━┓
	//┃0	                ┃
	//┃代码段  基地址为    ┃
	//┃64KB   user_base    ┃
	//┃                    ┃
	//┃64KB   HRB映像的值为┃
	//┃       0            ┃
	//┃数据段              ┃
	//┃堆栈段              ┃
	//┃192KB<-----ESP      ┃
	//┃                    ┃
	//┃数据段和堆栈段为同一┃
	//┃地址空间和长度，分别┃
	//┃两个描述符          ┃
	//┗━━━━━━━━━━┛
	u_int8 desc[8];
	u_int32 valid_pid=get_valid_pid();
	assert(valid_pid!=FALSE);
	/*初始化时间片*/
	process_table[valid_pid].max_time=max_time;
	process_table[valid_pid].current_time=max_time;
	
	u_int32 data_len  =*(u_int32*)(hrb_buf+16);//得到进程的实际数据段大小
	u_int32 data_pos  =*(u_int32*)(hrb_buf+20);//数据在可执行文件中的偏移
	u_int32 data_start=*(u_int32*)(hrb_buf+12);//得到数据在数据段中的开始位置
	/*该进程的特权级,createProcess函数无法创建特权级为1的进程，只有createServer才行*/
assert(DPL==2||DPL==3||DPL==1);
	u_int32 proc_dpl,proc_rpl;
	if(DPL==1)
	{
		proc_dpl=SegDesc_Property_DPL1;
		proc_rpl=Selector_RPL1;
	}
	else if(DPL==2)
	{
		proc_dpl=SegDesc_Property_DPL2;
		proc_rpl=Selector_RPL2;
	}
	else if(DPL==3)
	{
		proc_dpl=SegDesc_Property_DPL3;
		proc_rpl=Selector_RPL3;
	}
	else
	{
		return FALSE;
	}
	//建立LDT代码段
	/*建立LDT描述符*/
	gen_normalDescriptor(desc,user_base,allo_len,SegDesc_Property_EXEC_R|SegDesc_Property_32|proc_dpl);
	memcpy8(desc,process_table[valid_pid].ldtDescriptor[0],8);
	/*拷贝代码*****/
	memcpy8((u_int8*)hrb_buf,(u_int8*)user_base,buf_len);
	process_table[valid_pid].codeBase=user_base;
	user_base+=allo_len+1;
	
	//建立LDT堆栈段
	gen_normalDescriptor(desc,user_base,allo_len_4KB,SegDesc_Property_4KB|SegDesc_Property_RW|SegDesc_Property_32|proc_dpl    );
	memcpy8(desc,process_table[valid_pid].ldtDescriptor[1],8);
	process_table[valid_pid].stackBase=user_base;
	//base+=allo_len+1;
	
	
	//建立LDT数据段
	/*建立LDT描述符*/
	gen_normalDescriptor(desc,user_base,allo_len_4KB,SegDesc_Property_4KB|SegDesc_Property_RW|SegDesc_Property_32|proc_dpl    );
	memcpy8(desc,process_table[valid_pid].ldtDescriptor[2],8);
	/*拷贝数据*****/
	memcpy8((u_int8*)(hrb_buf+data_pos),(u_int8*)(user_base+data_start),data_len);
	process_table[valid_pid].dataBase=user_base;
	user_base+=(2*allo_len+2);
	
	
	/*初始化进程初始信息*/
	process_table[valid_pid].frame.esp   =0x20000;
	process_table[valid_pid].frame.eip   =0x001b;
	process_table[valid_pid].frame.eflags=INIT_EFLAGS;
	process_table[valid_pid].frame.ss    =1*8+Selector_TI_Ldt+proc_rpl;
	process_table[valid_pid].frame.cs    =0*8+Selector_TI_Ldt+proc_rpl;
	process_table[valid_pid].frame.ds    =2*8+Selector_TI_Ldt+proc_rpl;
	process_table[valid_pid].pid         =valid_pid;
	process_table[valid_pid].status      =STATUS_NO_TIME;
	//ldtDescriptor
	process_table[valid_pid].ldtSelector=addDes((u_int32)process_table[valid_pid].ldtDescriptor,3*8-1,SegDesc_Property_LDT);
	return valid_pid;
}
PUBLIC void init_process_table(                                                           ){
	/*进程表所有表项无效化*/
	int i;
	for(i=0;i<MAX_PROCESS;i++)
		process_table[i].status=STATUS_INVALID;
}
PUBLIC void sleep             (u_int32 pid,u_int32 status                                 ){
	/*仅仅是改变进程的状态*/
	process_table[pid].status=status;
}
PUBLIC void awake             (u_int32 pid,u_int32 parm                                   ){
	/*仅仅是改变进程的状态*/
	process_table[pid].status=STATUS_NO_TIME;
	process_table[pid].frame.eax=parm;
}
