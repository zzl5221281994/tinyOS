#ifndef message_H
#define message_H
#include "F:\work\tolset\tinyOS\kernel\lib\global.h                 "
/*ϵͳ����̶�pid*/
#define HD_DRIVER 1
#define FILE_SYS  2
#define SYSTEM    3
/***************************************************************************************/
//Ӳ����Ϣ�ṹ
#define HD_READ           1
#define HD_WRITE          2
#define HD_IDENTIFY       4
struct HD_MSG{
	u_int32 lba       ;
	void*   buf       ;//512�ֽ�
	u_int32 type      ;	
};
//Ӳ�̲����ṹ
struct HD_INFO{
	int8 hd_no[21];
	int8 hd_type[41];
	u_int32 sectorNum;
	u_int32 capabilities;
	u_int32 support_cmd;
};
/***************************************************************************************/

/*�ļ���Ϣ�ṹ*/
#define MAX_FILE_NAME 20
#define FILE_IDENTIFY 1
#define FILE_OPEN     2
#define FILE_READ     3
#define FILE_WRITE    4
#define FILE_CREATE   5
#define FILE_DELETE   6
#define POWER_OFF     7

struct I_NODE{/*һ��iNode����һ���ļ�*/
	int8 file_name[MAX_FILE_NAME];
	u_int32 byte_size  ;
	u_int32 startSector;
	u_int32 sectorNum  ;
	u_int32 status     ;
	u_int32 flags      ;
};
struct FILE_MSG{
	u_int32 type;
	u_int32 buf_len;
	u_int32 handle ;
	int8*file_name ;
	void*buf       ;
	struct I_NODE*inode;
	
};
/***************************************************************************************/
//������Ϣ�ṹ
#define CREATE_WND   1
#define UPDATE_FRAME 2
struct FRAME{
	void*buf;
	u_int32 startPosX;    //���λ��
	u_int32 startPosY;    //
	u_int32 height;
	u_int32 width;
};
struct WINDOW{
	u_int32 height;
	u_int32 width;
	u_int32  backGround;
	u_int8 *name;
};
struct WD_MSG{
	u_int32 type   ;
    struct WINDOW wnd;
	struct FRAME  frame;
};
/***************************************************************************************/
//�ж���Ϣ�ṹ
struct INT_MSG{
	u_int32 intNo;
    u_int32 status;	
};
/***************************************************************************************/
//��Ϣ����
#define HD_MSG_TYPE   1
#define WD_MSG_TYPE   2 
#define INT_MSG_TYPE  3
#define FILE_MSG_TYPE 4
#define MSG_STATUS_VALID     1
#define MSG_STATUS_INVALID   0

#define BLOCK_NEED           1
#define BLOCK_NOT_NEED       0
struct MESSAGE{
	u_int32 send_pid  ;
	u_int32 recv_pid  ;
	u_int32 msg_status;
	u_int32 type      ;
	u_int32 block     ;/*�����̷�������Ϣ�󣬸��ݴ��ֶ��ж��Ƿ���Ҫ�������ͽ���
	                     �����̴�������Ϣ�󣬸��ݴ��ֶ��ж��Ƿ���Ҫ���ѷ��ͽ���*/
	union {
		struct HD_MSG   msg_hd;
		struct WD_MSG   msg_wd;
		struct INT_MSG msg_int;
		struct FILE_MSG msg_file;
	} u;
};
extern void make_msg(struct MESSAGE *msg,u_int32 send_pid,u_int32 recv_pid,u_int32 type,u_int32 block);
#endif
