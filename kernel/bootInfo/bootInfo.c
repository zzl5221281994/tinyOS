#include "H:\work\tolset\tinyOS\kernel\lib\tinyOS.h"
#include "H:\work\tolset\tinyOS\kernel\graphics\font.h"
#define memory_Map_Length 20
#define bootInfo_Pointer  0x7000
#define bootInfo_memMap   0x701c
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
struct bootInfo boot_info;
int init_bootInfo(                                                                  ){
    char*bp=(char*)bootInfo_Pointer,*mp=(char*)bootInfo_memMap;
	boot_info.vram         = (unsigned int*)(*(unsigned int*)(bp+0));
	boot_info.screen_height=*(unsigned int*)(bp+4);
	boot_info.screen_width =*(unsigned int*)(bp+8);
	boot_info.mp_length    =*(unsigned int*)(bp+12);
	boot_info.codeBase     =*(unsigned int*)(bp+16);
	boot_info.dataBase     =*(unsigned int*)(bp+20);
	if(boot_info.mp_length>=memory_Map_Length)
		return FALSE;
	int i;
	for(i=0;i<boot_info.mp_length;i++)
	{
		boot_info.mp[i].BaseAddrLow  =*(unsigned int*)(mp+0 );
		boot_info.mp[i].BaseAddrHigh =*(unsigned int*)(mp+4 );
		boot_info.mp[i].lengthLow    =*(unsigned int*)(mp+8 );
		boot_info.mp[i].lengthHigh   =*(unsigned int*)(mp+12);
		boot_info.mp[i].Type         =*(unsigned int*)(mp+16);
		mp+=20;
	}
	return TRUE;
}