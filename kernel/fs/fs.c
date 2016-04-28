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
#include "F:\work\tolset\tinyOS\kernel\user_lib\user_lib.h        "
#include "F:\work\tolset\tinyOS\kernel\lib\string.h               "
/***********************************************************/
//文件系统的第一个扇区是superBlock，第二个扇区开始是扇区位图信息
//
//
/***********************************************************/
#define MAX_FILE_NAME   20
#define MAX_FILE        100
#define MAX_SECTOR      81920
#define MAX_BMP_SECTOR  (MAX_SECTOR/4096)
/***********************************************************/
#define I_NODE_INVALID  0
#define I_NODE_VALID    1
struct i_node{
	int8 file_name[MAX_FILE_NAME];
	u_int32 startSector;
	u_int32 sectorNum  ;
	u_int32 status     ;
	u_int32 flags      ;
};
struct FAT{
	struct i_node i_node_arr[MAX_FILE];
	u_int32 size                      ;
};
/*文件系统收发消息结构*/
struct MESSAGE msg_send;
struct MESSAGE msg_recv;

/***********************/
struct HD_INFO hd_info      ;      /*硬盘信息    */
struct FILE_SYS_INFO fs_info;      /*文件系统信息*/
struct FAT   fat            ;      /*文件分配表  */                          

u_int16  superBlock[256];
u_int8       hd_bmp[MAX_SECTOR/8];//格式为位图
u_int32 fs_pid;
u_int32 identify_hd (                );
u_int32 identify_fs (                );
void    create_fs   (                );
void    read_fs_info(                );
void    read_hd_bmp (                );
u_int32 read_sector (u_int32 lba,void*buf);
u_int32 write_sector(u_int32 lba,void*buf);

/*文件操作函数*/
u_int32 identify_file();
u_int32 open_file    ();
u_int32 read_file    ();
u_int32 write_file   ();
u_int32 create_file  ();
u_int32 delete_file  ();
/*main*/
void HariMain(void){
	fs_pid=get_pid();
	while(identify_hd()==FALSE);
	while(identify_fs()==FALSE);//读取superBlock
	/*如果硬盘上不存在文件系统，则新建一个。注：约定tinyOS文件系统的superBlock前5字节为字符串“TINY”*/
	if(strcmp((int8*)superBlock,"TINY")==FALSE)
		create_fs();
	
	read_fs_info();
	read_hd_bmp ();
	drawNum(sizeof(struct FAT),100,100);
	/*进入文件系统消息循环*/
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
				switch(file_msg_type)
				{
				    case FILE_IDENTIFY:
				                      if(identify_file()==TRUE )
										  awake(send_pid,TRUE  );
									  else
										  awake(send_pid,FALSE );
								      break;
				    case FILE_OPEN    :
				                      if(open_file()==TRUE     )
										   awake(send_pid,TRUE );
									  else
										   awake(send_pid,FALSE);
								      break;
				    case FILE_READ    :
				                      if(read_file()==TRUE     )
										   awake(send_pid,TRUE );
									  else
										   awake(send_pid,FALSE);
								      break;
				    case FILE_WRITE   :
				                      if(write_file()==TRUE    )
										   awake(send_pid,TRUE );
									  else
										   awake(send_pid,FALSE);
								      break;
				    case FILE_CREATE  :
				                      if(create_file()==TRUE   )
										   awake(send_pid,TRUE );
									  else
										   awake(send_pid,FALSE);
									  
								      break;
				    case FILE_DELETE  :
				                      if(delete_file()==TRUE   )
										   awake(send_pid,TRUE );
									  else
										   awake(send_pid,FALSE);
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
		return TRUE;
	}
	
}
u_int32 identify_fs     (                                     ){
	u_int32 status=read_sector(0,superBlock);
	if(status==FALSE)
		return FALSE;
	else
		return TRUE;
}
void read_hd_bmp        (                                     ){
	int8 buf[512]; 
	u_int32 sectorNum=hd_info.sectorNum;
	u_int32 read_times=((sectorNum%4096)==0)?(sectorNum/4096):(sectorNum/4096+1);
	u_int32 i;
	for(i=0;i<read_times;i++)
	{
		while(read_sector(1+i,buf)==FALSE);
		memcpy8(buf,hd_bmp+(i<<9),512);
	}
}
void create_fs          (                                     ){
	
}
void read_fs_info       (                                     ){
	
}
u_int32 identify_file   (                                     ){
	return 0;
}
u_int32 open_file       (                                     ){
	return 0;
}
u_int32 read_file       (                                     ){
	return 0;
}
u_int32 write_file      (                                     ){
	return 0;
}
u_int32 create_file     (                                     ){
	return 0;
}
u_int32 delete_file     (                                     ){
	return 0;
}
u_int32 read_sector     (u_int32 lba,void*buf                 ){
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
u_int32 write_sector    (u_int32 lba,void*buf                 ){
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
