#ifndef bootInfo_h
#define bootInfo_h

#define max_gdtDescriptor 256
#define max_idtDescriptor 256
#define memory_Map_Length 20
#define bootInfo_Pointer  0x7000
#define bootInfo_memMap   0x7020
struct main_gdt             {
	unsigned char  gdtDescriptor[max_gdtDescriptor][8];
	unsigned char  gdt_ptr[6]                         ;
    unsigned int   gdtBase                            ;	
	unsigned int   gdtLimit                           ;
	unsigned int   gdtDescriptor_length               ;
};
struct main_idt             {
	unsigned char  idtDescriptor[max_idtDescriptor][8];
	unsigned int   idtDescriptor_length;
};
struct addr_range_descriptor{
	unsigned int   BaseAddrLow ;
	unsigned int   BaseAddrHigh;
	unsigned int   lengthLow   ;
	unsigned int   lengthHigh  ;
	unsigned int   Type        ;
};
struct bootInfo{
	unsigned int   *vram                                 ;
	unsigned int   screen_height                         ;
	unsigned int   screen_width                          ;
	unsigned int   mp_ptr_length                         ;
    unsigned int   codeBase                              ;
    unsigned int   dataBase                              ;
    unsigned int   *label_gdt                            ;
    unsigned int   *numOfGdts                            ;	
    struct   addr_range_descriptor mp[memory_Map_Length] ;
};
extern int init_bootInfo(                                                             );
extern int init_gdt     (                                                             );
extern int init_idt     (                                                             );
extern int set_pattle   (                                                             );
#endif
