#include "hd.h                                                    "
#include "F:\work\tolset\tinyOS\kernel\user_lib\user_lib.h        "
#include "F:\work\tolset\tinyOS\kernel\user_lib\user_sys_call.h   "
#define DRIVER   0
#define HD_INTNO 14
PRIVATE u_int32 hd_pid;
PRIVATE u_int8 init_hd         (                                                 );
PRIVATE void hd_cmd_out        (struct hd_cmd* cmd                               );
PRIVATE void hd_identify       (u_int32 driver,u_int32 awake_pid,u_int8*liner_buf); 
PRIVATE u_int32 hd_sector_read (u_int32 lba,u_int32 awake_pid   ,u_int8*liner_buf);
PRIVATE u_int32 hd_sector_write(u_int32 lba,u_int32 awake_pid   ,u_int8*liner_buf);
struct MESSAGE msg_recv;
struct MESSAGE msg_send;
void HariMain(void){
	hd_pid=get_pid();
	u_int32 hd=init_hd();
	if(hd==0)
	{
		while(1)
		{
			receive(&msg_recv,STATUS_RECV_ANY,0,hd_pid);
			awake(msg_recv.send_pid,FALSE);
		}
	}
	//test
	u_int32 num=0;
	//test
	while(1)
	{
		    receive(&msg_recv,STATUS_RECV_ANY,0,hd_pid);
			u_int32 msg_type=msg_recv.type;
			u_int32 send_pid=msg_recv.send_pid;
			switch(msg_type)
	        {
				case HD_MSG:
				{
					u_int32 hd_type=msg_recv.u.msg_hd.type;
					u_int32 lba    =msg_recv.u.msg_hd.lba ;
					void*buf       =(void*)transTolinerAddr((u_int32)msg_recv.u.msg_hd.buf,send_pid,1);
					switch(hd_type)
					{
						case HD_IDENTIFY:
						                 hd_identify(DRIVER,send_pid,buf);
						                 break;
						case HD_READ    :
						                 hd_sector_read(lba,send_pid,buf);
										 break;
					    case HD_WRITE   :
						                hd_sector_write(lba,send_pid,buf);
						default         :
						                awake(send_pid,FALSE);
						                break;
					}
				}
		        default:
					awake(send_pid,FALSE);
		            break;
	        }
	}
}
PRIVATE u_int8 init_hd           (                               ){
	u_int8 *ptr=(u_int8*)0x475;//此处保存着机器上硬盘的数量
	return *ptr;
}
PRIVATE void hd_identify       (u_int32 driver,u_int32 awake_pid,u_int8*liner_buf){
	assert(liner_buf!=NULL);
	struct hd_cmd cmd;
	cmd.device  =MAKE_DEVICE_REG(LBA_MODE,driver/*主硬盘0号控制器*/,0);
	cmd.command =ATA_IDENTIFY;
	hd_cmd_out(&cmd);
	struct MESSAGE intMsg;
	receive(&intMsg,STATUS_RECV_INT,HD_INTNO,hd_pid);/*等待硬盘中断唤醒*/
	
	/*读取硬盘数据*/
	int i;
	u_int16*buf=(u_int16*)liner_buf;
	for(i=0;i<256;i++)
		buf[i]=io_in16(REG_DATA);
	awake(awake_pid,TRUE);
}
PRIVATE u_int32 hd_sector_read (u_int32 lba,u_int32 awake_pid,u_int8*liner_buf){
	assert(liner_buf!=NULL);
	struct hd_cmd cmd;
	cmd.features=0;
	cmd.device  =MAKE_DEVICE_REG(LBA_MODE,0/*主硬盘0号控制器*/,/*lba高4位*/(lba&0xf000000)>>24);
	cmd.command =ATA_READ;
	cmd.count   =1;/*只允许读取一个扇区*/
	cmd.lba_low =lba&0xff;
	cmd.lba_mid =((lba&0xff00)>>8);
	cmd.lba_high=((lba&0xff0000)>>16);
	struct MESSAGE intMsg;
	receive(&intMsg,STATUS_RECV_INT,HD_INTNO,hd_pid);/*等待硬盘中断唤醒*/
	
	/*读取硬盘数据*/
	int i;
	u_int16*buf=(u_int16*)liner_buf;
	for(i=0;i<256;i++)
		buf[i]=io_in16(REG_DATA);
	awake(awake_pid,TRUE);
	return TRUE;
}
PRIVATE u_int32 hd_sector_write(u_int32 lba,u_int32 awake_pid,u_int8*liner_buf){
	assert(liner_buf!=NULL);
	struct hd_cmd cmd;
	
	cmd.features=0;
	cmd.device  =MAKE_DEVICE_REG(LBA_MODE,0/*主硬盘0号控制器*/,/*lba高4位*/(lba&0xf000000)>>24);
	cmd.command =ATA_WRITE;
	cmd.count   =1;/*只允许写入一个扇区*/
	cmd.lba_low =lba&0xff;
	cmd.lba_mid =((lba&0xff00)>>8);
	cmd.lba_high=((lba&0xff0000)>>16);
	hd_cmd_out(&cmd);
	//等待驱动器设置DRQ数据请求信号，然后写入数据
	while((io_in8(REG_STATUS)&STATUS_DRQ)==0);
	u_int16*buf=(u_int16*)liner_buf;
	int i;
	for(i=0;i<256;i++)
		io_out16(REG_DATA,buf[i]);
	/*数据输出完毕,等待中断唤醒*/
	struct MESSAGE intMsg;
	receive(&intMsg,STATUS_RECV_INT,HD_INTNO,hd_pid);/*等待硬盘中断唤醒*/
	awake(awake_pid,TRUE);
	return TRUE;
}
PRIVATE void hd_cmd_out        (struct hd_cmd* cmd          ){

	/* Activate the Interrupt Enable (nIEN) bit */
	io_out8(REG_DEV_CTRL, 0);
	/* Load required parameters in the Command Block Registers */
	io_out8(REG_FEATURES, cmd->features);
	io_out8(REG_NSECTOR,  cmd->count);
	io_out8(REG_LBA_LOW,  cmd->lba_low);
	io_out8(REG_LBA_MID,  cmd->lba_mid);
	io_out8(REG_LBA_HIGH, cmd->lba_high);
	io_out8(REG_DEVICE,   cmd->device);
	while((io_in8(REG_STATUS)&STATUS_DRDY)==0);
	/* Write the command code to the Command Register */
	io_out8(REG_CMD,     cmd->command);
}
