#include "process.h"
#include "F:\work\tolset\tinyOS\kernel\bootInfo\bootInfo.h          "
#include "F:\work\tolset\tinyOS\kernel\lib\string.h                 "
#include "F:\work\tolset\tinyOS\kernel\kernelFun.h                  "
PUBLIC struct proc_table process_table[MAX_PROCESS];
PUBLIC struct TSS global_tss;
PUBLIC u_int32 current_exec_pid=0;
PUBLIC u_int32 process_table_len=1;
PRIVATE u_int32 base=0x1000000;
PRIVATE u_int32 allo_len=0xFFFF;
PUBLIC void schedule();

//test
extern void testA();
extern void testB();
//test
void init_tss(){
	global_tss.ss0=KERNEL_STACK_INDEX*8;
	global_tss.esp0=0x1000000;
	global_tss.iobase=104;
	u_int32 sel=addDes((u_int32)(&global_tss),103,SegDesc_Property_386TSS);
	/*int i;
	u_int32*ptr=(u_int32*)(&global_tss);
	for(i=0;i<26;i++)
		drawNum(ptr[i],300+i*16,0,0x3c,0x00);*/
	//drawNum(sel,500+current_exec_pid*1024,500+current_exec_pid*1024,0x3c,0x00);
	loadTss(sel*8);
	/*drawNum(12345678,500+current_exec_pid*1024,500+current_exec_pid*1024,0x1f,0x00);
	while(1)
		io_hlt();*/
}
void schedule(){
	current_exec_pid=(current_exec_pid+1)%3;
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
	
	drawNum(ss,500,0,0x3c,0x00);
	drawNum(esp,500,100,0x3c,0x00);
	drawNum(eflags,500,200,0x3c,0x00);
	drawNum(cs,500,300,0x3c,0x00);
	drawNum(eip,500,400,0x3c,0x00);
	
	drawNum(process_table[0].frame.ss,600,0,0x3c,0x00);
	drawNum(process_table[0].frame.esp,600,100,0x3c,0x00);
	drawNum(process_table[0].frame.eflags,600,200,0x3c,0x00);
	drawNum(process_table[0].frame.cs,600,300,0x3c,0x00);
	drawNum(process_table[0].frame.eip,600,400,0x3c,0x00);
	/*int i,j;
	for(i=0;i<gdt.gdtDescriptor_length;i++)
		for(j=0;j<8;j++)
			drawNum(gdt.gdtDescriptor[i][j],300+i*16,30*j,0x3c,0x00);*/
	//drawNum(current_exec_pid,600,0,0x3c,0x1f);
	loadReg(eip,cs,esp,ss,eax,ecx,edx,ebx,ebp,esi,edi,eflags);
	//while(1)
	//	io_hlt();
	/*u_int8*ptr=(u_int8*)(process_table[current_exec_pid].ldtDescriptor);
	int i,j;
	for(i=0;i<3;i++)
		for(j=0;j<8;j++)
			drawNum(process_table[current_exec_pid].ldtDescriptor[i][j],500+current_exec_pid*1024+i*16,j*30,0x3c,0x1f);
	while(1)
		io_hlt();*/
	//runProcess(eip,cs,esp,ss);
}
void storeFrame(u_int32 frame[]){
	int i;
	for(i=0;i<7;i++)
		drawNum(frame[i],400+current_exec_pid*32,i*100,0x3c,0x00);
	for(i=7;i<13;i++)
		drawNum(frame[i],400+current_exec_pid*32+16,(i-7)*100,0x3c,0x00);
	process_table[current_exec_pid].frame.edi=frame[0];
	process_table[current_exec_pid].frame.esi=frame[1];
	process_table[current_exec_pid].frame.ebp=frame[2];
	
	process_table[current_exec_pid].frame.ebx=frame[4];
	process_table[current_exec_pid].frame.edx=frame[5];
	process_table[current_exec_pid].frame.ecx=frame[6];
	process_table[current_exec_pid].frame.eax=frame[7];
	process_table[current_exec_pid].frame.eip=frame[8];
	process_table[current_exec_pid].frame.cs=frame[9];
	process_table[current_exec_pid].frame.eflags=frame[10];
	if(current_exec_pid==0)
	{
		process_table[current_exec_pid].frame.esp=frame[3]+12;
		process_table[current_exec_pid].frame.ss=KERNEL_STACK_INDEX*8;
	}
	else
	{
		process_table[current_exec_pid].frame.esp=frame[11];
		process_table[current_exec_pid].frame.ss=frame[12];
	}
	schedule();
}
int32 createProcess(u_int32 key){
	u_int8 desc[8];
	//代码
	gen_normalDescriptor(desc,base,allo_len,SegDesc_Property_EXEC_R|SegDesc_Property_32|SegDesc_Property_DPL1);
	memcpy8(desc,process_table[process_table_len].ldtDescriptor[0],8);
	if(key==1)
		memcpy8((u_int8*)(boot_info.codeBase+testA),(u_int8*)base,0xffff);
	else
		memcpy8((u_int8*)(boot_info.codeBase+testB),(u_int8*)base,0xffff);
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
	process_table[process_table_len].frame.eflags=0x00001202;
	process_table[process_table_len].frame.ss=2*8+Selector_TI_Ldt+Selector_RPL1;
	process_table[process_table_len].frame.cs=0*8+Selector_TI_Ldt+Selector_RPL1;
	process_table[process_table_len].frame.ds=KERNEL_DATA_INDEX*8+Selector_RPL1;
	process_table[process_table_len].pid=process_table_len;
	//ldtDescriptor
	process_table[process_table_len].ldtSelector=addDes((u_int32)process_table[process_table_len].ldtDescriptor,3*8-1,SegDesc_Property_LDT);
	return process_table_len++;
}
