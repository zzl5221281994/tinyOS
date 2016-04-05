#ifndef bootInfo.h
#define bootInfo.h
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
extern struct bootInfo boot_info;
extern int init_bootInfo(struct bootInfo*p                                         );
#endif