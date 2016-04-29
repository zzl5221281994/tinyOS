/************************************************************************************
Wed Apr 27 10:19:58 2016

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
#ifndef fs_H
#define fs_H
#include "F:\work\tolset\tinyOS\kernel\user_lib\user_lib.h        "
#include "F:\work\tolset\tinyOS\kernel\lib\string.h               "
/***********************************************************/
//文件系统的第二个扇区是superBlock，第三个扇区开始是扇区位图信息
//LBA:0    boot sector（占用1个扇区 ）
//LBA:1    superBlock （占用1个扇区 ）
//LBA:2~21 扇区位图   （占用20个扇区）
//LBA:22   文件分配表 （占用10个扇区）
//LBA:31   数据区
/***********************************************************/
#define SUPERBLOCK_SIZE sizeof(struct SUPERBLOCK)
#define     I_NODE_SIZE sizeof(struct I_NODE    )
#define        FAT_SIZE sizeof(struct FAT       )

#define MAX_FILE_NAME    20
#define MAX_FILE         100

#define BOOTSECTOR_START 0
#define SUPERBLOCK_START 1
#define HD_BMP_START     2
#define FAT_START        22
#define DATA_START       32

#define MAX_FAT          10
#define MAX_SECTOR       81920
#define MAX_BMP_SECTOR  (MAX_SECTOR/4096)
/***********************************************************/
#define I_NODE_INVALID  0
#define I_NODE_VALID    1
#define STATUS_OPEN     2

#define ALLOC_FILE_LEN  50
struct SUPERBLOCK{
	char sign[5];
	u_int32 n_sects;
	u_int32 n_inode;
};
struct FAT{
	struct I_NODE i_node_arr[MAX_FILE];
	u_int32 size                      ;
};
extern u_int32 identify_hd (                );
extern u_int32 identify_fs (                );
extern void    create_fs   (                );
extern void    read_fat    (                );
extern u_int32 read_hd_bmp (                );
extern u_int32 read_sector (u_int32 lba,void*buf);
extern u_int32 write_sector(u_int32 lba,void*buf);

/*文件操作函数*/
extern u_int32 identify_file(u_int32 handle,struct I_NODE*inode,u_int32 send_pid          );
extern u_int32 open_file    (int8*file_name,u_int32 send_pid                              );
extern u_int32 read_file    (u_int32 handle,u_int32 buf_len,void*buf ,u_int32 send_pid    );
extern u_int32 write_file   (u_int32 handle,u_int32 buf_len,void*buf,u_int32 send_pid     );
extern u_int32 create_file  (int8* file_name,u_int32 send_pid                             );
extern u_int32 delete_file  (u_int32 handle ,u_int32 send_pid                             );

/*函数*/
extern void write_back           (/*关机之前写回表格信息*/           );
extern u_int32 get_bit           (u_int32 lba                        );
extern u_int32 get_valid_inode   (                                   );
extern u_int32 get_valid_sector  (                                   );
extern void mask0                (u_int32 lba                        );
extern void mask1                (u_int32 lba                        );
#endif
