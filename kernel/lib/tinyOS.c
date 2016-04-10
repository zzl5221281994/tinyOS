/*
%macro Descriptor 3
		dw %2 & 0FFFFh
		dw %1 & 0FFFFh
		db (%1>>16) & 0FFh
		dw ((%2>>8) & 0f00h) | (%3 & 0f0ffh)
		db (%1>>24) & 0ffh
%endmacro
*/
void gen_code_dataDescriptor(unsigned char desc[8],unsigned int base,unsigned int limit,unsigned short attribute){
	unsigned short bit1=limit&0xffff;
	desc[0]= bit1&0x00ff;
	desc[1]=(bit1&0xff00)>>8;
	unsigned short bit2= base&0xffff;
	desc[2]= bit2&0x00ff;
	desc[3]=(bit2&0xff00)>>8;
	unsigned char  bit3=(base>>16)&0x00ff;
	desc[4]=bit3;
	unsigned short bit4=((limit>>8)&0xf00)|(attribute&0xf0ff);
	desc[5]=bit4&0x00ff;
	desc[6]=(bit4&0xff00)>>8;
	unsigned char  bit5=(base>>24)&0xff;
	desc[7]=bit5;
	
}
void      gen_sys_Descriptor(unsigned char desc[8],unsigned int base,unsigned int limit,unsigned int attribute){
	
}