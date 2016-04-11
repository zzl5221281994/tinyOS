#ifndef bootInfo_h
#define bootInfo_h

#define max_gdtDescriptor 256
#define max_idtDescriptor 256
#define memory_Map_Length 20
#define bootInfo_Pointer  0x7000
#define bootInfo_memMap   0x7020
struct main_gdt             {
	u_int8  gdtDescriptor[max_gdtDescriptor][8];
	u_int8  gdt_ptr[6]                         ;
    u_int32   gdtBase                          ;	
	u_int32   gdtLimit                         ;
	u_int32   gdtDescriptor_length             ;
};
struct main_idt             {
	u_int8    idtDescriptor[max_idtDescriptor][8];
	u_int32   idtDescriptor_length               ;
};
struct addr_range_descriptor{
	u_int32   BaseAddrLow ;
	u_int32   BaseAddrHigh;
	u_int32   lengthLow   ;
	u_int32   lengthHigh  ;
	u_int32   Type        ;
};
struct bootInfo{
	u_int32   *vram                                 ;
	u_int32   screen_height                         ;
	u_int32   screen_width                          ;
	u_int32   mp_ptr_length                         ;
    u_int32   codeBase                              ;
    u_int32   dataBase                              ;
    u_int32   *label_gdt                            ;
    u_int32   *numOfGdts                            ;	
    struct   addr_range_descriptor mp[memory_Map_Length] ;
};
extern int init_bootInfo(                                                             );
extern int init_gdt     (                                                             );
extern int init_idt     (                                                             );
extern int set_pattle   (                                                             );
#endif
