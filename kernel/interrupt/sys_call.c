/************************************************************************************
Tue Apr 19 10:32:53 2016

MIT License
Copyright (c) 2016 zhuzuolang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************************/
#include "sys_call.h                                            "
#include "F:\work\tolset\tinyOS\kernel\lib\string.h             "
#include "F:\work\tolset\tinyOS\kernel\multiTask\process.h      "
#include "F:\work\tolset\tinyOS\kernel\multiTask\message.h      "
#include "F:\work\tolset\tinyOS\kernel\bootInfo\bootInfo.h      "
/*系统调用数组*/
PUBLIC u_int32 sys_call_table[SYS_CALL_NUM];
/*进程表的消息队列数组*/
#define MAX_MSG_QUEUE  36
PRIVATE struct MESSAGE default_msg_queue[MAX_PROCESS][MAX_MSG_QUEUE]; /*pid  4~24*/
PUBLIC u_int32 send_msg(struct MESSAGE*msg,u_int32 call_pid  ){
	assert(msg!=NULL);
	assert((call_pid>=0)&&(call_pid<MAX_PROCESS));
	//struct MESSAGE m;/*通过调用门访问，msg为进程的局部地址，需转换为线性地址*/
	struct MESSAGE *m_ptr=(struct MESSAGE*)l_addr2liner_addr((u_int32)msg,call_pid,1);
	u_int32 recv_pid=m_ptr->recv_pid;
	u_int32 send_pid=m_ptr->send_pid;
	assert(recv_pid!=send_pid);
	
	u_int32 msg_type=m_ptr->type              ;
	u_int32 intNo   =m_ptr->u.msg_int.intNo   ;/*当该消息是中断类型时有用*/
    u_int32 block   =m_ptr->block             ;
	//test
	//test
 	u_int32 status=process_table[recv_pid].status;

	if(status==STATUS_RECV_ANY)
	{/*如果接收进程被阻塞，并处于可接受任意进程发送的消息时*/
		memcpy8((u_int8*)m_ptr,(u_int8*)((process_table[recv_pid].msg)),sizeof(struct MESSAGE));/*将消息拷入msg*/
		awake(recv_pid,STATUS_RECV_ANY);
		if(block==BLOCK_NEED)/*是否需要阻塞*/
		{
			sleep(call_pid,STATUS_SEND_PENDING);
			schedule();
		}
		else if(block==BLOCK_NOT_NEED)
		{
			return TRUE;
		}
	}
	else if(status==STATUS_RECV_SPECIFY&&process_table[recv_pid].specify==send_pid)
	{/*如果接收进程被阻塞，并处于可接受指定进程发送的消息且当前消息就是该指定进程发送时*/
		memcpy8((u_int8*)m_ptr,(u_int8*)((process_table[recv_pid].msg)),sizeof(struct MESSAGE));
		awake(recv_pid,STATUS_RECV_SPECIFY);
		if(block==BLOCK_NEED)/*是否需要阻塞*/
		{
			sleep(call_pid,STATUS_SEND_PENDING);
			schedule();
		}
		else if(block==BLOCK_NOT_NEED)
		{
			return TRUE;
		}
	}
	else if(status==STATUS_RECV_INT&&send_pid==0&&msg_type==INT_MSG_TYPE&&process_table[recv_pid].specify==intNo)
	{/*如果接收进程被阻塞，并处于等待中断发生的状态*/
		memcpy8((u_int8*)m_ptr,(u_int8*)((process_table[recv_pid].msg)),sizeof(struct MESSAGE));
		awake(recv_pid,STATUS_RECV_INT);
		if(block==BLOCK_NEED)/*是否需要阻塞*/
		{
			sleep(call_pid,STATUS_SEND_PENDING);
			schedule();
		}
		else if(block==BLOCK_NOT_NEED)
		{
			return TRUE;
		}
	}
	else
	{/*接收进程处于未接收消息状态，将消息放入队列*/
	    /*由于该消息并未被接收，所以发送进程应当被阻塞并执行调度程序*/
		process_table[recv_pid].msg_queue_size++;
		if(msg_type!=INT_MSG_TYPE)/*如果消息不是中断类型，则一定是某个进程发送的*/
		{
			memcpy8((u_int8*)m_ptr,(u_int8*)(&(process_table[recv_pid].msg_queue[send_pid+16])),sizeof(struct MESSAGE));
			if(block==BLOCK_NEED)/*是否需要阻塞*/
			{
				sleep(send_pid,STATUS_SEND_PENDING);
		        schedule();
			}
			else if(block==BLOCK_NOT_NEED)
			{
				return TRUE;
			}
		}
		else/*消息是中断类型,中断类型是由内核发送的，一定不能阻塞*/
		{
			
			memcpy8((u_int8*)m_ptr,(u_int8*)(&(process_table[recv_pid].msg_queue[intNo])),sizeof(struct MESSAGE));
			return TRUE;
		}
	}
	return FALSE;
}
PUBLIC u_int32 recv_msg(struct MESSAGE*msg,u_int32 type,u_int32 specify,u_int32 call_pid){
	/*type=STATUS_RECV_ANY，    specify无效  */
	/*type=STATUS_RECV_SPECIFY，specify=pid  */
	/*type=STATUS_RECV_INT,     specify=intNo*/
	assert(msg!=NULL);
	assert((call_pid>=0)&&(call_pid<MAX_PROCESS));
	assert((type==STATUS_RECV_ANY)||(type==STATUS_RECV_SPECIFY)||(type==STATUS_RECV_INT));
	//struct MESSAGE m;/*通过调用门访问，msg为进程的局部地址，需转换为线性地址*/
	process_table[call_pid].msg=(struct MESSAGE*)l_addr2liner_addr((u_int32)msg,call_pid,1);
	if(process_table[call_pid].msg_queue_size==0)
	{
		process_table[call_pid].specify=specify;
		sleep(call_pid,type);
		schedule();
	}
	else if(type==STATUS_RECV_ANY)
	{
		/*队列里有消息并且欲接收任意进程发送的消息，STATUS_RECV_ANY暂定包含中断类型*/
		int i;
		for(i=0;i<MAX_MSG_QUEUE;i++)
			if(process_table[call_pid].msg_queue[i].msg_status==MSG_STATUS_VALID)
			{
				memcpy8((u_int8*)(&(process_table[call_pid].msg_queue[i])),(u_int8*)process_table[call_pid].msg,sizeof(struct MESSAGE));
				process_table[call_pid].msg_queue[i].msg_status=MSG_STATUS_INVALID;
			    process_table[call_pid].msg_queue_size--;
				return TRUE;
			}
	}
	else if(type==STATUS_RECV_SPECIFY)/*想接受特定进程发送的消息*/
	{
		if(process_table[call_pid].msg_queue[specify+16].msg_status==MSG_STATUS_INVALID)/*特定进程并未发送消息*/
		{
			process_table[call_pid].specify=specify;
			sleep(call_pid,STATUS_RECV_SPECIFY);
			schedule();
		}
		else/*接收成功*/
		{
			memcpy8((u_int8*)(&(process_table[call_pid].msg_queue[specify+16])),(u_int8*)process_table[call_pid].msg,sizeof(struct MESSAGE));
			process_table[call_pid].msg_queue[specify+16].msg_status=MSG_STATUS_INVALID;
			process_table[call_pid].msg_queue_size--;
			return TRUE;
		}
	}
	else if(type==STATUS_RECV_INT)/*欲接收指定中断号码的消息*/
	{
		if(process_table[call_pid].msg_queue[specify].msg_status==MSG_STATUS_INVALID)/*特定中断并未发送消息*/
		{
			process_table[call_pid].specify=specify;
			sleep(call_pid,STATUS_RECV_INT);
			schedule();
		}
		else/*接收成功*/
		{
			memcpy8((u_int8*)(&(process_table[call_pid].msg_queue[specify])),(u_int8*)process_table[call_pid].msg,sizeof(struct MESSAGE));
			process_table[call_pid].msg_queue[specify].msg_status=MSG_STATUS_INVALID;
			process_table[call_pid].msg_queue_size--;
			return TRUE;
		}
	}
	return FALSE;
}

PUBLIC void init_msg_queue      (                                     ){
	/*应该完成消息缓冲区的无效化并初始化进程表的缓冲区字段*/
	int i,j;
	for(i=0;i<MAX_PROCESS;i++)
		for(j=0;j<MAX_MSG_QUEUE;j++)
			default_msg_queue[i][j].msg_status=MSG_STATUS_INVALID;
	for(i=0;i<MAX_PROCESS;i++)
	{
		process_table[i].msg_queue=&default_msg_queue[i][0];
		process_table[i].msg_queue_size=0;
	}
	
}
//TEST
PRIVATE u_int32 draw(u_int8* str,u_int32 key,u_int32 pid){
	u_int8*ptr=(u_int8*)l_addr2liner_addr((u_int32)str,pid,1);
	if(key==0)
		drawStr(ptr,0,100,0x2e,0x3c);
	else if(key==1)
		drawStr(ptr,200,100,0x1e,0x3c);
	return TRUE;
	
}
PRIVATE u_int32 get_pid        (                               ){
	return current_exec_pid;
}
PRIVATE void get_screen_info   (u_int32 addr ,u_int32 pid      ){
	u_int32*ptr=(u_int32*)l_addr2liner_addr(addr,pid,1);
	*ptr=(u_int32)boot_info.vram;
	*(ptr+1)=boot_info.screen_width;
	*(ptr+2)=boot_info.screen_height;
}
PRIVATE void dNum             (u_int32 num,u_int32 x,u_int32 y ){
	drawNum(num,x,y,0x1f,0x00);
}
//TEST
PUBLIC void init_sys_call_table (                                     ){
	sys_call_table[0]=(u_int32)send_msg;
	sys_call_table[1]=(u_int32)recv_msg;
	//test
	sys_call_table[2]=(u_int32)sleep            ;
	sys_call_table[3]=(u_int32)awake            ;
	sys_call_table[4]=(u_int32)draw             ;
	sys_call_table[5]=(u_int32)l_addr2liner_addr;
	sys_call_table[6]=(u_int32)get_pid          ;
	sys_call_table[7]=(u_int32)assertion_failure;
	sys_call_table[8]=(u_int32)dNum             ;
	sys_call_table[9]=(u_int32)get_screen_info  ;
	return;
}
PUBLIC u_int32 l_addr2liner_addr(u_int32 addr,u_int32 pid,u_int32 type){
	if(type==0)
		return process_table[pid].codeBase+addr ;
	else if(type==1)
		return process_table[pid].dataBase+addr ;
	else if(type==2)
		return process_table[pid].stackBase+addr;
	else
		return FALSE;
}
