#include "F:\work\tolset\zzlOS\kernel\lib\zzlOS.h"
#include "F:\work\tolset\zzlOS\kernel\graphics\font.h"
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
    struct addr_range_descriptor mp[memory_Map_Length] ;
};
struct bootInfo boot_info;
int init_bootInfo(/*struct bootInfo*p */                                      ){
    char*bp=bootInfo_Pointer,*mp=bootInfo_memMap;
	boot_info.vram         =*(unsigned int*)(bp+0);
	boot_info.screen_height=*(unsigned int*)(bp+4);
	boot_info.screen_width =*(unsigned int*)(bp+8);
	boot_info.mp_length    =*(unsigned int*)(bp+12);
	if(boot_info.mp_length>=memory_Map_Length)
		return zzlOS_ERROR;
	int i,j;
	for(i=0;i<boot_info.mp_length;i++)
	{
		boot_info.mp[i].BaseAddrLow  =*(unsigned int*)(mp+0 );
		boot_info.mp[i].BaseAddrHigh =*(unsigned int*)(mp+4 );
		boot_info.mp[i].lengthLow    =*(unsigned int*)(mp+8 );
		boot_info.mp[i].lengthHigh   =*(unsigned int*)(mp+12);
		boot_info.mp[i].Type         =*(unsigned int*)(mp+16);
		mp+=20;
	}
	return zzlOS_SUCCESS;
}