#include "process.h"
#include "F:\work\tolset\tinyOS\kernel\bootInfo\bootInfo.h          "
#include "F:\work\tolset\tinyOS\kernel\lib\string.h                 "
#include "F:\work\tolset\tinyOS\kernel\kernelFun.h                  "
PUBLIC struct proc_table process_table[MAX_PROCESS];
PUBLIC struct TSS global_tss                       ;
PUBLIC u_int32 current_exec_pid=0                  ;
PUBLIC u_int32 interrupt_mutex=0                   ;
PUBLIC u_int32 process_table_len=1                 ;
PRIVATE u_int32 base    =0x1000000                 ;
PRIVATE u_int32 allo_len=0xFFFF                    ;

PUBLIC void init_tss(){
	u_int32 tss_sel=addDes(0,0xfffff,SegDesc_Property_32 |SegDesc_Property_4KB|SegDesc_Property_RW);
	global_tss.ss0=tss_sel*8;
	global_tss.esp0=0x1000000;
	global_tss.iobase=104;
	u_int32 sel=addDes((u_int32)(&global_tss),103,SegDesc_Property_386TSS);
	loadTss(sel*8);
}
PUBLIC void schedule(){
	process_table[current_exec_pid].current_time--;
	if(process_table[current_exec_pid].current_time==0)
	{
		process_table[current_exec_pid].current_time=process_table[current_exec_pid].max_time;
		current_exec_pid=(current_exec_pid+1)%process_table_len;
	}
	if(current_exec_pid==0)
		current_exec_pid++;
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
	//loadReg(eip,cs,esp,ss,eax,ecx,edx,ebx,ebp,esi,edi,eflags&0xfffffdff);//eflags & 0xfffffdff ,关中断
	loadReg(eip,cs,eflags,esp,ss,eax,ecx,edx,ebx,ebp,esi,edi);
}
PUBLIC int32 createProcess(u_int32 offset,u_int32 max_time){
	u_int8 desc[8];
	process_table[process_table_len].max_time=max_time;
	process_table[process_table_len].current_time=max_time;
	//代码
	gen_normalDescriptor(desc,base,allo_len,SegDesc_Property_EXEC_R|SegDesc_Property_32|SegDesc_Property_DPL1);
	memcpy8(desc,process_table[process_table_len].ldtDescriptor[0],8);
	memcpy8((u_int8*)offset,(u_int8*)base,0xffff);
	base+=allo_len+1;
	//数据
	gen_normalDescriptor(desc,base,allo_len,SegDesc_Property_RW|SegDesc_Property_32|SegDesc_Property_DPL1);
	memcpy8(desc,process_table[process_table_len].ldtDescriptor[1],8);
	base+=allo_len+1;
	//堆栈
	gen_normalDescriptor(desc,base,allo_len,SegDesc_Property_RW|SegDesc_Property_32|SegDesc_Property_DPL1);
	memcpy8(desc,process_table[process_table_len].ldtDescriptor[2],8);
	base+=allo_len+1;
	
	
	process_table[process_table_len].frame.esp=0xffff;
	process_table[process_table_len].frame.eip=0x0000;
	process_table[process_table_len].frame.eflags=INIT_EFLAGS;
	process_table[process_table_len].frame.ss=2*8+Selector_TI_Ldt+Selector_RPL1;
	process_table[process_table_len].frame.cs=0*8+Selector_TI_Ldt+Selector_RPL1;
	process_table[process_table_len].frame.ds=KERNEL_DATA_INDEX*8+Selector_RPL1;
	process_table[process_table_len].pid=process_table_len;
	//ldtDescriptor
	process_table[process_table_len].ldtSelector=addDes((u_int32)process_table[process_table_len].ldtDescriptor,3*8-1,SegDesc_Property_LDT);
	return process_table_len++;
}
