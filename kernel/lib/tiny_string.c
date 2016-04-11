#include "tinyOS.h"
u_int32 tiny_strlen  (int8*str                                                   ){
	int32 len=0;
	while((*str)!='\0')
	{
		str++;
		len++;
	}   
	return len;
}
void intToStr        (u_int32 num ,u_int8*  desc ,int32   bufSize				 ){
	int32 pos=bufSize-1;
	if(num==0)
	{
		desc[0]='0';
		desc[1]='\0';
		return;
	}
	while(num>0){
		int32 current=num%10;
		desc[pos]='0'+current;
		pos--;
		num=num/10;
	}
	pos++;
	int32 len=0,i;
	for(i=pos;i<=bufSize-1;i++)
		desc[len++]=desc[i];
	desc[len]='\0';
}
int32 memcpy8        (u_int8*  src,u_int8*  desc ,u_int32 buffSize               ){
	u_int32 i=0;
	if(src==NULL||desc==NULL)
		return FALSE;
	while((i++)<buffSize)
		(*(desc++))=(*(src++));
	return TRUE;
}
int32 memcpy32       (u_int32* src,u_int32* desc ,u_int32 buffSize               ){
	u_int32 i=0;
	if(src==NULL||desc==NULL)
		return FALSE;
	while((i++)<buffSize)
		(*(desc++))=(*(src++));
	return TRUE;
}