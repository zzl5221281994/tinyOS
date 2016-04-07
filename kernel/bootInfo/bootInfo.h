#ifndef bootInfo_h
#define bootInfo_h
/******************************************************************************************
;段描述符共8字节

;  BYTE7         BYTE6 BYTE5                               BYTE4 BYTE3 BYTE2    BYTE1 BYTE0
;  段基址2     /   段属性    \                              |    段基址1    |      段界限
;  31~24      /               \                                23  ~    0           15~0
;            /                 \
;           /                   \
;         7   6   5  4       3 2 1 0    7  6 5  4  3 2 1 0
;         G D/B   0  AVL    |段界限2|   P  DPL  S   TYPE
;                             19~16
;P位：存在位，是否在内存，1表示存在     G位：段界限粒度，0表示粒度为字节，1表示粒度为4KB
;DPL：特权级，0~3 ，越小特权级越大      S位：S=1表示数据段/代码段，S=0表示系统段/门描述符
;D/B：   
;
;
;
;
;*****************************************************************************************/
//;存储段描述符属性
#define SegDesc_Property_4KB                    0x8000  //;4KB粒度
#define SegDesc_Property_32                     0x4000  //;32位段

#define SegDesc_Property_DPL0                   0x0000  //;DPL：特权级=0
#define SegDesc_Property_DPL1                   0x0020  //;DPL：特权级=1
#define SegDesc_Property_DPL2                   0x0040  //;DPL：特权级=2
#define SegDesc_Property_DPL3                   0x0060  //;DPL：特权级=3

#define SegDesc_Property_R                      0x0090  //;存在的只读数据段	
#define SegDesc_Property_RW                     0x0092  //;存在的可读写数据段
#define SegDesc_Property_RWA                    0x0093  //;存在的已访问可读写数据段	
#define SegDesc_Property_EXEC                   0x0098  //;存在的只执行代码段	
#define SegDesc_Property_EXEC_R                 0x009A  //;存在的可执行可读代码段		
#define SegDesc_Property_EXEC_C                 0x009C  //;存在的可执行一致代码段	
#define SegDesc_Property_EXEC_RC                0x009E  //;存在的可读可执行一致代码段	
//;系统段描述符
#define SegDesc_Property_LDT                    0x0082  //;局部描述符表段
#define SegDesc_Property_TaskGate               0x0085  //;任务门段
#define SegDesc_Property_386TSS                 0x0089  //;386任务状态段
#define SegDesc_Property_386CGate               0x008C  //;386调用门段
#define SegDesc_Property_386IGate               0x008E  //;386中断门段
#define SegDesc_Property_TGate                  0x008F  //;386陷阱门段

KERNEL_SIZE						EQU		12000
//;内存分布图数据结构入口*****************************************************************/
#define memory_Map_Length 20
struct addr_range_descriptor{
	unsigned int BaseAddrLow ;
	unsigned int BaseAddrHigh;
	unsigned int lengthLow   ;
	unsigned int lengthHigh  ;
	unsigned int Type        ;
};
struct bootInfo{
	unsigned int *vram                                 ;
	int       screen_height                            ;
	int       screen_width                             ;
	int       mp_length                                ;
    unsigned int codeBase                              ;
    unsigned int dataBase                              ;	
    struct addr_range_descriptor mp[memory_Map_Length] ;
};
extern struct bootInfo boot_info;
extern int init_bootInfo(                                                             );
extern int set_pattle   (                                                             );
#endif
