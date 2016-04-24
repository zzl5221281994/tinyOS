#include "process.h"
#include "F:\work\tolset\tinyOS\kernel\bootInfo\bootInfo.h          "
#include "F:\work\tolset\tinyOS\kernel\lib\string.h                 "
#include "F:\work\tolset\tinyOS\kernel\debug\debug.h                "
#include "F:\work\tolset\tinyOS\kernel\kernelFun.h                  "
PUBLIC struct proc_table process_table[MAX_PROCESS];
PUBLIC struct TSS global_tss                       ;
PUBLIC u_int32 current_exec_pid=0                  ;
PUBLIC u_int32 process_table_len=1                 ;
PRIVATE u_int32 base    =0x1000000                 ;
PRIVATE u_int32 allo_len=0xFFFF                    ;

PUBLIC void init_tss(){
	/*全局只有一个TSS*/
	u_int32 tss_sel=addDes(0,0xfffff,SegDesc_Property_32 |SegDesc_Property_4KB|SegDesc_Property_RW);
	global_tss.ss0=tss_sel*8;
	global_tss.esp0=0x1000000;
	global_tss.iobase=104;
	u_int32 sel=addDes((u_int32)(&global_tss),103,SegDesc_Property_386TSS);
	loadTss(sel*8);
}
PUBLIC void schedule(){
	/*进程调度*/
	process_table[current_exec_pid].current_time--;
	if(process_table[current_exec_pid].current_time==0)
	{
		process_table[current_exec_pid].current_time=process_table[current_exec_pid].max_time;
		current_exec_pid=(current_exec_pid+1)%process_table_len;
	}
	if(current_exec_pid==0)
		current_exec_pid++;
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
PUBLIC int32 createServer(u_int32 offset,u_int32 max_time){
	u_int8 desc[8];
	process_table[process_table_len].max_time=max_time;
	process_table[process_table_len].current_time=max_time;
	//建立LDT代码段
	gen_normalDescriptor(desc,base,allo_len,SegDesc_Property_EXEC_R|SegDesc_Property_32|SegDesc_Property_DPL1);
	memcpy8(desc,process_table[process_table_len].ldtDescriptor[0],8);
	memcpy8((u_int8*)offset,(u_int8*)base,0xffff);
	process_table[process_table_len].codeBase=base;
	base+=allo_len+1;
	//建立LDT数据段
	gen_normalDescriptor(desc,base,allo_len,SegDesc_Property_RW|SegDesc_Property_32|SegDesc_Property_DPL1);
	memcpy8(desc,process_table[process_table_len].ldtDescriptor[1],8);
	process_table[process_table_len].dataBase=base;
	base+=allo_len+1;
	
	//建立LDT堆栈段
	gen_normalDescriptor(desc,base,allo_len,SegDesc_Property_RW|SegDesc_Property_32|SegDesc_Property_DPL1);
	memcpy8(desc,process_table[process_table_len].ldtDescriptor[2],8);
	process_table[process_table_len].stackBase=base;
	base+=allo_len+1;
	
	/*初始化进程初始信息*/
	process_table[process_table_len].frame.esp   =0xffff;
	process_table[process_table_len].frame.eip   =0x0000;
	process_table[process_table_len].frame.eflags=INIT_EFLAGS;
	process_table[process_table_len].frame.ss    =2*8+Selector_TI_Ldt+Selector_RPL1;
	process_table[process_table_len].frame.cs    =0*8+Selector_TI_Ldt+Selector_RPL1;
	process_table[process_table_len].frame.ds    =KERNEL_DATA_INDEX*8+Selector_RPL1;
	process_table[process_table_len].pid         =process_table_len;
	process_table[process_table_len].status      =STATUS_NO_TIME;
	//ldtDescriptor
	process_table[process_table_len].ldtSelector=addDes((u_int32)process_table[process_table_len].ldtDescriptor,3*8-1,SegDesc_Property_LDT);
	return process_table_len++;
}
PUBLIC int32 createProcess(u_int8*hrb_buf,u_int32 buf_len,u_int32 max_time,u_int32 DPL){
	u_int8 desc[8];
	/*初始化时间片*/
	process_table[process_table_len].max_time=max_time;
	process_table[process_table_len].current_time=max_time;
	
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
	
	//建立LDT代码段
	/*建立LDT描述符*/
	gen_normalDescriptor(desc,base,allo_len,SegDesc_Property_EXEC_R|SegDesc_Property_32|proc_dpl);
	memcpy8(desc,process_table[process_table_len].ldtDescriptor[0],8);
	/*拷贝代码*****/
	memcpy8((u_int8*)hrb_buf,(u_int8*)base,buf_len);
	process_table[process_table_len].codeBase=base;
	base+=allo_len+1;
	
	//建立LDT数据段
	/*建立LDT描述符*/
	gen_normalDescriptor(desc,base,allo_len,SegDesc_Property_RW|SegDesc_Property_32|proc_dpl    );
	memcpy8(desc,process_table[process_table_len].ldtDescriptor[1],8);
	/*拷贝数据*****/
	memcpy8((u_int8*)(hrb_buf+data_pos),(u_int8*)(base+data_start),data_len);
	process_table[process_table_len].dataBase=base;
	base+=allo_len+1;
	
	//建立LDT堆栈段
	gen_normalDescriptor(desc,base,allo_len,SegDesc_Property_RW|SegDesc_Property_32|proc_dpl    );
	memcpy8(desc,process_table[process_table_len].ldtDescriptor[2],8);
	process_table[process_table_len].stackBase=base;
	base+=allo_len+1;
	
	/*初始化进程初始信息*/
	process_table[process_table_len].frame.esp   =0xffff;
	process_table[process_table_len].frame.eip   =0x001b;
	process_table[process_table_len].frame.eflags=INIT_EFLAGS;
	process_table[process_table_len].frame.ss    =2*8+Selector_TI_Ldt+proc_rpl;
	process_table[process_table_len].frame.cs    =0*8+Selector_TI_Ldt+proc_rpl;
	process_table[process_table_len].frame.ds    =KERNEL_DATA_INDEX*8+proc_rpl;
	//process_table[process_table_len].frame.ds    =1*8+Selector_TI_Ldt+proc_rpl;
	process_table[process_table_len].pid         =process_table_len;
	process_table[process_table_len].status      =STATUS_NO_TIME;
	//ldtDescriptor
	process_table[process_table_len].ldtSelector=addDes((u_int32)process_table[process_table_len].ldtDescriptor,3*8-1,SegDesc_Property_LDT);
	return process_table_len++;
}
PUBLIC void sleep            (u_int32 pid,u_int32 status    ){/*仅仅是改变进程的状态*/
	process_table[pid].status=status;
}
PUBLIC void awake            (u_int32 pid,u_int32 parm      ){/*仅仅是改变进程的状态*/
	process_table[pid].status=STATUS_NO_TIME;
	process_table[pid].frame.eax=parm;
}