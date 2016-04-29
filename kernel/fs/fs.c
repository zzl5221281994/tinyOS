/************************************************************************************
Wed Apr 27 10:19:16 2016

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
#include "fs.h"
/*文件系统收发消息结构*/
struct MESSAGE msg_send;
struct MESSAGE msg_recv;

/***********************/
struct HD_INFO        hd_info    ;      /*硬盘信息      */
struct SUPERBLOCK     superBlock ;      /*超级块        */
u_int8   hd_bmp[MAX_SECTOR/8]    ;      /*扇区位图      */
u_int32  bmp_sect                ;      /*位图实际扇区数*/
struct FAT            fat        ;      /*文件分配表    */

u_int32  fs_pid                  ;
/*main*/
void HariMain(void){
	fs_pid=get_pid();
	while(identify_hd()==FALSE);
	while(identify_fs()==FALSE);//读取superBlock
	/*如果硬盘上不存在文件系统，则新建一个。注：约定tinyOS文件系统的superBlock前5字节为字符串“TINY”*/
	if(strcmp(superBlock.sign,"TINY")==FALSE)
	{
		create_fs();
		identify_fs();/*因为之前读取的可能无效，当文件系统不存在时*/
	}
	read_hd_bmp ();
	read_fat();
	
	while(1)
	{
		receive(&msg_recv,STATUS_RECV_ANY,0,fs_pid);
		u_int32 recv_type=msg_recv.type;
		u_int32 send_pid =msg_recv.send_pid;
		switch(recv_type)
		{
			case FILE_MSG_TYPE:
			{
				struct FILE_MSG *msg =&msg_recv.u.msg_file;
				u_int32 file_msg_type=msg->type           ;
				int8*file_name       =msg->file_name      ;
				void*buf             =msg->buf            ;
				u_int32 buf_len      =msg->buf_len        ;
				u_int32  handle      =msg->handle         ;
				u_int32 result                            ;
				struct I_NODE*inode  =msg->inode          ;
				switch(file_msg_type)
				{
				    case FILE_IDENTIFY:
				                      if(identify_file(handle,inode,send_pid)==TRUE )
										  awake(send_pid,TRUE  );
									  else
										  awake(send_pid,FALSE );
								      break;
				    case FILE_OPEN    :
					                  handle=open_file(file_name,send_pid);
									  awake(send_pid,handle);
								      break;
				    case FILE_READ    :
				                      if((result=read_file(handle,buf_len,buf,send_pid))!=FALSE)
										   awake(send_pid,result );
									  else
										   awake(send_pid,FALSE  );
								      break;
				    case FILE_WRITE   :
				                      if(write_file(handle,buf_len,buf,send_pid)==TRUE    )
										   awake(send_pid,TRUE );
									  else
										   awake(send_pid,FALSE);
								      break;
				    case FILE_CREATE  :
				                      if(create_file(file_name,send_pid)==TRUE   )
										   awake(send_pid,TRUE );
									  else
										   awake(send_pid,FALSE);
									  
								      break;
				    case FILE_DELETE  :
				                      if(delete_file(handle,send_pid)==TRUE    )
										   awake(send_pid,TRUE  );
									  else
										   awake(send_pid,FALSE );
								      break;
					case POWER_OFF    :
					                   write_back(              );
									   awake(send_pid,TRUE      );
									   break;
				    default:
				                      awake(send_pid,FALSE     );
								      break;
			    }
				break;
		    }
			default:
			    break;
		
	    }
	
    }
}
u_int32 identify_hd     (                                     ){
	u_int16 hd_info_buf[256];
	struct HD_MSG hd;
	hd.lba=0;
	hd.buf=hd_info_buf;
	hd.type=HD_IDENTIFY;
	make_msg(&msg_send,fs_pid,HD_DRIVER,HD_MSG_TYPE,BLOCK_NEED);
	msg_send.u.msg_hd=hd;
	u_int32 key=send(&msg_send,fs_pid);
	if(key==FALSE)
		return 0;
	else
	{
		memcpy8((u_int8*)(hd_info_buf+10),(u_int8*)hd_info.hd_no,20);//硬盘序列号
		hd_info.hd_no[20]='\0';
		hd_info.hd_type[40]='\0';
		memcpy8((u_int8*)(hd_info_buf+27),(u_int8*)hd_info.hd_type,40);//型号
		hd_info.sectorNum=*((u_int32*)(hd_info_buf+60));
		hd_info.capabilities=*(hd_info_buf+49);
		hd_info.support_cmd=*(hd_info_buf+83);
		bmp_sect=((hd_info.sectorNum%4096)==0)?(hd_info.sectorNum/4096):(hd_info.sectorNum/4096+1);
		return TRUE;
	}
	
}
u_int32 identify_fs     (                                     ){
	int8 buf[512];
	u_int32 status=read_sector(SUPERBLOCK_START,buf);
	if(status==FALSE)
		return FALSE;
	else
	{
		memcpy8(buf,(u_int8*)&superBlock,SUPERBLOCK_SIZE);
		return TRUE;
	}
}
u_int32 read_hd_bmp        (                                     ){
	int8 buf[512]; 
	u_int32 read_times=bmp_sect;
	assert(read_times<=MAX_BMP_SECTOR);
	
	u_int32 i;
	for(i=0;i<read_times;i++)
	{
		while(read_sector(HD_BMP_START+i,buf)==FALSE);
		memcpy8(buf,hd_bmp+(i<<9),512);
	}
	return TRUE;
}
void create_fs          (                                     ){
	/*进入此函数证明当前硬盘无任何操作系统，需要初始化superBlock，扇区位图，文件分配表*/
	/*superBlock*/
	memcpy8("TINY",superBlock.sign,5);/*文件系统标识*/
	superBlock.n_sects=hd_info.sectorNum;
	superBlock.n_inode=MAX_FILE;
	
	/*扇区位图，注：前32个扇区存储文件系统的信息，一定处于使用状态*/
	int i;
	for(i=0;i<DATA_START;i++)
		mask1(i);
	for(i=DATA_START;i<superBlock.n_sects;i++)
		mask0(i);
	/*文件分配表,初始时全部无效化*/
	fat.size=0;
	for(i=0;i<MAX_FILE;i++)
		fat.i_node_arr[i].status=I_NODE_INVALID;
	/*将信息全部写入硬盘*/
	u_int8 buf[512];
	/*写superBlock*/
	memcpy8((u_int8*)&superBlock,buf,SUPERBLOCK_SIZE);
	while(write_sector(SUPERBLOCK_START,buf)==FALSE);
	/*写位图*/
	u_int32 bmp_sect=((superBlock.n_sects%4096)==0)?(superBlock.n_sects/4096):(superBlock.n_sects/4096+1);
	
	u_int32 pos=0;
	for(i=HD_BMP_START;i<HD_BMP_START+bmp_sect;i++,pos+=512)
		while(write_sector(i,hd_bmp+pos)==FALSE);
	
	/*写文件分配表*/
	pos=0;
	for(i=FAT_START;i<FAT_START+MAX_FAT;i++,pos+=512)
		while(write_sector(i,((u_int8*)&fat+pos))==FALSE);
}
void read_fat           (                                     ){
	int i;
	u_int8 buf[512];
	u_int32 write_pos=0;
	for(i=FAT_START;i<FAT_START+MAX_FAT;i++)
	{
		while(read_sector(i,buf)==FALSE);
		/*注：由于最后一个扇区可能只有一部分是FAT的内容，为保证写入不越界*/
		if(write_pos+512<=FAT_SIZE)/*如果当前读入的扇区全部是fat内容*/
		{
			memcpy8(buf,(u_int8*)&fat+write_pos,512);
			write_pos+=512;
		}
		else/*否则当前buf只有FAT_SIZE-write_pos个字节属于fat*/
		{
			memcpy8(buf,(u_int8*)&fat+write_pos,FAT_SIZE-write_pos);
			return;
		}
	}
		
}
/*文件操作函数*/
u_int32 identify_file   (u_int32 handle,struct I_NODE*inode,u_int32 send_pid      ){
	/*patch：判断handle的有效性*/
	int8*temp=(int8*)transTolinerAddr((u_int32)inode,send_pid,1);
	memcpy8((u_int8*)&fat.i_node_arr[handle],temp,sizeof(struct I_NODE));
	return TRUE;
}
u_int32 open_file       ( int8*file_name,u_int32 send_pid     ){
	int i;
	int8*name=transTolinerAddr(file_name,send_pid,1);
	for(i=1;i<MAX_FILE;i++)
		if(fat.i_node_arr[i].status==I_NODE_VALID&&(strcmp(name,fat.i_node_arr[i].file_name)==TRUE))
			return i;
	
	return FALSE;
}
u_int32 read_file       (u_int32 handle,u_int32 buf_len,void*buf ,u_int32 send_pid){
	/*patch：判断权限*/
	int i;
	int8*buffer=(int8*)transTolinerAddr((u_int32)buf,send_pid,1);
	/*patch：判断handle的有效性*/
	u_int32 size=fat.i_node_arr[handle].byte_size;
	/*请求长度超过了文件长度，只读取本文件*/
	if(buf_len>size)
		buf_len=size;
	u_int32 read_sects=buf_len/512;
	u_int32 start=fat.i_node_arr[handle].startSector;
	u_int32 pos=0;
	for(i=start;i<start+read_sects;i++,pos+=512)
		while(read_sector(i,buffer+pos)==FALSE);
	if((buf_len%512)==0)
	{
		return buf_len;
	}
	else
	{/*否则对最后一个扇区特殊处理*/
		int8 tempBuf[512];
		while(read_sector(start+read_sects,tempBuf)==FALSE);
		memcpy8(tempBuf,buffer+read_sects*512,buf_len%512);
		return buf_len;
	}
}
u_int32 write_file      (u_int32 handle,u_int32 buf_len,void*buf,u_int32 send_pid ){
	/*patch：判断权限*/
	int i;
	/*patch：判断handle的有效性*/
	int8*buffer=(int8*)transTolinerAddr((u_int32)buf,send_pid,1);
	u_int32 write_sects=buf_len/512;
	/*patch:增加判断扇区超出文件拥有扇区*/
	u_int32 start=fat.i_node_arr[handle].startSector;
	u_int32 pos=0;
	for(i=start;i<start+write_sects;i++,pos+=512)
		while(write_sector(i,buffer+pos)==FALSE);
	/*文件若变得更长，则需要更新信息*/
	u_int32 size=fat.i_node_arr[handle].byte_size;
	if(buf_len>size)
		fat.i_node_arr[handle].byte_size=buf_len;
	if((buf_len%512)==0)
	{
		return TRUE;
	}
	else
	{/*否则对最后一个扇区特殊处理*/
		int8 tempBuf[512];
		while(read_sector(start+write_sects,tempBuf)==FALSE);
		memcpy8(buffer+write_sects*512,tempBuf,buf_len%512);
		while(write_sector(start+write_sects,tempBuf)==FALSE);
		return TRUE;
	}
}
u_int32 create_file     (int8* file_name,u_int32 send_pid                        ){
	int8*ptr=transTolinerAddr((u_int32)file_name,send_pid,1);
	u_int32 i_node=get_valid_inode();
	if(i_node==0)
		return FALSE;
	u_int32 len=strlen(ptr);
	if(len>=MAX_FILE_NAME)
		return 0;
	memcpy8(ptr,fat.i_node_arr[i_node].file_name,len);
	fat.i_node_arr[i_node].file_name[len]='\0';
	/*分配文件的扇区，并设置位图*/
	u_int32 valid_sector=get_valid_sector();
	fat.i_node_arr[i_node].byte_size=0;
	fat.i_node_arr[i_node].startSector=valid_sector;
	fat.i_node_arr[i_node].sectorNum=ALLOC_FILE_LEN;
	fat.i_node_arr[i_node].status=I_NODE_VALID;
	int i;
	for(i=valid_sector;i<valid_sector+ALLOC_FILE_LEN;i++)
		mask1(i);
	fat.size++;
	return TRUE;
}
u_int32 delete_file     ( u_int32 handle ,u_int32 send_pid                       ){
	/*patch：判断权限*/
	/*清除文件分配表表项*/
	int i;
	u_int32 start=fat.i_node_arr[handle].startSector;
	u_int32 n_sects=fat.i_node_arr[handle].sectorNum;
	
	fat.i_node_arr[handle].byte_size=0;
	fat.i_node_arr[handle].startSector=0;
	fat.i_node_arr[handle].sectorNum=0;
	fat.i_node_arr[handle].status=I_NODE_INVALID;
	fat.size--;
	for(i=0;i<MAX_FILE_NAME;i++)
		fat.i_node_arr[handle].file_name[i]='\0';
	for(i=start;i<start+n_sects;i++)
		mask0(i);
	
	return TRUE;
}
/*函数*/
void write_back           (/*关机之前写回表格信息*/                              ){
	/*写superBlock*/
	while(write_sector(SUPERBLOCK_START,(u_int8*)&superBlock)==FALSE);
	/*写位图*/
	u_int32 bmp_sect=((superBlock.n_sects%4096)==0)?(superBlock.n_sects/4096):(superBlock.n_sects/4096+1);
	
	u_int32 pos=0,i;
	for(i=HD_BMP_START;i<HD_BMP_START+bmp_sect;i++,pos+=512)
		while(write_sector(i,hd_bmp+pos)==FALSE);
	
	/*写文件分配表*/
	pos=0;
	for(i=FAT_START;i<FAT_START+MAX_FAT;i++,pos+=512)
		while(write_sector(i,((u_int8*)&fat+pos))==FALSE);
}
u_int32 get_bit         (u_int32 lba                                             ){
	return hd_bmp[lba/8]&(1<<(lba%8));
}
u_int32 get_valid_inode (                                                        ){
	int i;
	for(i=1;i<MAX_FILE;i++)
		if(fat.i_node_arr[i].status==I_NODE_INVALID)
			return i;
	return FALSE;
}
u_int32 get_valid_sector(                                                        ){
	u_int32 i,j;
	for(i=DATA_START;i<MAX_SECTOR-ALLOC_FILE_LEN;i+=ALLOC_FILE_LEN)
	{
		u_int32 mark=0;
		for(j=i;j<i+ALLOC_FILE_LEN;j++)
		{
			if(get_bit(j)!=0)
			{
				mark=1;
				break;
			}
		}
		if(mark==0)
			return i;
	}
	return FALSE;
}
void mask0              (u_int32 lba                                             ){
	u_int32 addr=lba/8;
	hd_bmp[addr]=hd_bmp[addr]&(~(1<<(lba%8)));
}
void mask1              (u_int32 lba                                             ){
	u_int32 addr=lba/8;
	hd_bmp[addr]=hd_bmp[addr]|(1<<(lba%8));
}
u_int32 read_sector     (u_int32 lba,void*buf                                    ){
	struct HD_MSG hd;
	hd.lba=lba;
	hd.buf=buf;
	hd.type=HD_READ;
	make_msg(&msg_send,fs_pid,HD_DRIVER,HD_MSG_TYPE,BLOCK_NEED);
	msg_send.u.msg_hd=hd;
	u_int32 key=send(&msg_send,fs_pid);
	if(key==TRUE)
		return TRUE;
	else
		return FALSE;
}
u_int32 write_sector    (u_int32 lba,void*buf                                    ){
	struct HD_MSG hd;
	hd.lba=lba;
	hd.buf=buf;
	hd.type=HD_WRITE;
	make_msg(&msg_send,fs_pid,HD_DRIVER,HD_MSG_TYPE,BLOCK_NEED);
	msg_send.u.msg_hd=hd;
	u_int32 key=send(&msg_send,fs_pid);
	if(key==TRUE)
		return TRUE;
	else
		return FALSE;
}
