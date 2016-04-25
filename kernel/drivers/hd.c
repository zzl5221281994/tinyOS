#include "hd.h                                               "
#include "F:\work\tolset\tinyOS\kernel\debug\debug.h         "
#include "F:\work\tolset\tinyOS\kernel\multiTask\process.h   "
#include "F:\work\tolset\tinyOS\kernel\sys.h                 "
PRIVATE u_int32 hd_pid;
PRIVATE void hd_cmd_out        (struct hd_cmd* cmd                         );
PRIVATE void hd_identify       (u_int32 driver,u_int8*liner_buf            ); 
PRIVATE u_int32 hd_sector_read (u_int32 lba,u_int8*liner_buf               );
PRIVATE u_int32 hd_sector_write(u_int32 lba,u_int8*liner_buf               );
void HariMain(void){
	while(1)
	{
		    receive()
			switch(hardDisk_msg[temp].type)
	        {
				case HD_IDENTIFY:
					hd_identify(0,hardDisk_msg[temp].liner_buf,temp);
					break;
		        case HD_READ:
			        hd_sector_read(hardDisk_msg[temp].lba,hardDisk_msg[temp].liner_buf,temp);
			        break;
		        case HD_WRITE:
			        hd_sector_write(hardDisk_msg[temp].lba,hardDisk_msg[temp].liner_buf,temp);
			        break;
		        default:
		            break;
	        }
		}
		hardDisk_msg_pos=(hardDisk_msg_pos+1)%MAX_HD_MSG;
    }
}
PRIVATE void init_hd           (                               ){
	u_int8 *ptr=0x475;//此处保存着机器上硬盘的数量
	assertt((*ptr)!=0);//确认机器存在硬盘
	/*消息缓冲区全部无效*/
}
PRIVATE void hd_identify       (u_int32 driver,u_int8*liner_buf,u_int32 pos){
	struct hd_cmd cmd;
	cmd.device  =MAKE_DEVICE_REG(LBA_MODE,driver/*主硬盘0号控制器*/,0);
	cmd.command =ATA_IDENTIFY;
	hd_cmd_out(&cmd);
	sleep(hd_pid);
	int i;
	u_int16*buf=(u_int16*)liner_buf;
	for(i=0;i<256;i++)
		buf[i]=io_in8(REG_DATA);
	awake(hardDisk_msg[pos].pid);
}
PRIVATE u_int32 hd_sector_read (u_int32 lba,u_int8*liner_buf,u_int32 pos){
	struct hd_cmd cmd;
	cmd.features=0;
	cmd.device  =MAKE_DEVICE_REG(LBA_MODE,0/*主硬盘0号控制器*/,/*lba高4位*/(lba&0xf000000)>>24);
	cmd.command =ATA_READ;
	cmd.count   =1;/*只允许读取一个扇区*/
	cmd.lba_low =lba&0xff;
	cmd.lba_mid =((lba&0xff00)>>8);
	cmd.lba_high=((lba&0xff0000)>>16);
	hd_cmd_out(&cmd);
	sleep(hd_pid);
	int i;
	u_int16*buf=(u_int16*)liner_buf;
	for(i=0;i<256;i++)
		buf[i]=io_in8(REG_DATA);
	awake(hardDisk_msg[pos].pid);
}
PRIVATE u_int32 hd_sector_write(u_int32 lba,u_int8*liner_buf,u_int32 pos){
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
	while((io_in8(REG_STATUS)&STATUS_DRQ)==0);
	u_int16*ptr=(u_int16*)liner_buf;
	int i;
	for(i=0;i<256;i++)
		io_out16(REG_DATA,ptr[i]);
	/*数据输出完毕,等待中断唤醒*/
	sleep(hd_pid);
	awake(hardDisk_msg[pos].pid);
}
PRIVATE void hd_cmd_out        (struct hd_cmd* cmd          ){

	/* Activate the Interrupt Enable (nIEN) bit */
	out_byte(REG_DEV_CTRL, 0);
	/* Load required parameters in the Command Block Registers */
	out_byte(REG_FEATURES, cmd->features);
	out_byte(REG_NSECTOR,  cmd->count);
	out_byte(REG_LBA_LOW,  cmd->lba_low);
	out_byte(REG_LBA_MID,  cmd->lba_mid);
	out_byte(REG_LBA_HIGH, cmd->lba_high);
	out_byte(REG_DEVICE,   cmd->device);
	while((io_in8(REG_STATUS)&STATUS_DRDY)==0);
	/* Write the command code to the Command Register */
	out_byte(REG_CMD,     cmd->command);
}
}