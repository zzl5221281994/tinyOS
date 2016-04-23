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
#define MAX_MSG_QUEUE  25
PRIVATE struct MESSAGE default_msg_queue[MAX_PROCESS][MAX_MSG_QUEUE]; /*pid  4~24*/
PUBLIC u_int32 msg_queue_table[MAX_PROCESS];
PUBLIC u_int32 send_msg(struct MESSAGE*msg,u_int32 call_pid){
	assert(msg!=NULL);
	assert((call_pid>=0)&&(call_pid<MAX_PROCESS));
	//struct MESSAGE m;/*通过调用门访问，msg为进程的局部地址，需转换为线性地址*/
	struct MESSAGE *m_ptr=(struct MESSAGE*)l_addr2liner_addr((u_int32)msg,call_pid,1);
	u_int32 recv_pid=m_ptr->recv_pid;
	u_int32 send_pid=m_ptr->send_pid;
	assert(recv_pid!=send_pid);
	
	u_int32 msg_type=m_ptr->type              ;
	u_int32 intNo   =m_ptr->u.msg_int.intNo   ;/*当该消息是中断类型时有用*/

	u_int32 status=process_table[recv_pid].status;
	if(status==STATUS_RECV_ANY)
	{/*如果接收进程被阻塞，并处于可接受任意进程发送的消息时*/
		memcpy8((u_int8*)m_ptr,(u_int8*)(&(process_table[recv_pid].msg)),sizeof(struct MESSAGE));/*将消息拷入msg*/
		awake(recv_pid);
		return TRUE;
	}
	else if(status==STATUS_RECV_SPECIFY&&process_table[recv_pid].specify==send_pid)
	{/*如果接收进程被阻塞，并处于可接受指定进程发送的消息且当前消息就是该指定进程发送时*/
		memcpy8((u_int8*)m_ptr,(u_int8*)(&(process_table[recv_pid].msg)),sizeof(struct MESSAGE));
		awake(recv_pid);
		return TRUE;
	}
	else if(status==STATUS_RECV_INT&&send_pid==0&&msg_type==INT_MSG&&process_table[recv_pid].specify==intNo)
	{/*如果接收进程被阻塞，并处于等待中断发生的状态*/
		memcpy8((u_int8*)m_ptr,(u_int8*)(&(process_table[recv_pid].msg)),sizeof(struct MESSAGE));
		awake(recv_pid);
		return TRUE;
	}
	else
	{/*接收进程处于未接收消息状态，将消息放入队列*/
	    /*由于该消息并未被接收，所以发送进程应当被阻塞并执行调度程序*/
		
		memcpy8((u_int8*)m_ptr,(u_int8*)(&(process_table[recv_pid].msg_queue[send_pid])),sizeof(struct MESSAGE));
		sleep(send_pid,STATUS_SEND_PENDING);
		schedule();
	}
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
	else if(type==STATUS_RECV_SPECIFY)
	{
		if(process_table[call_pid].msg_queue[specify].msg_status==MSG_STATUS_INVALID)
		{
			process_table[call_pid].specify=specify;
			sleep(call_pid,STATUS_RECV_SPECIFY);
			schedule();
		}
		else
		{
			
		}
	}
}

PUBLIC void init_msg_queue      (                                     ){
	/*应该完成消息缓冲区的无效化并初始化进程表的缓冲区字段*/
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
