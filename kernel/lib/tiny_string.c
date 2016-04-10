#include "tinyOS.h"
int tiny_strlen  (unsigned char*str                                      ){
	int len=0;
	while((*str)!='\0')
	{
		str++;
		len++;
	}   
	return len;
}
void intToStr(unsigned int num,unsigned char*desc,int bufSize					     ){
	int pos=bufSize-1;
	if(num==0)
	{
		desc[0]='0';
		desc[1]='\0';
		return;
	}
	while(num>0){
		int current=num%10;
		desc[pos]='0'+current;
		pos--;
		num=num/10;
	}
	pos++;
	int len=0,i;
	for(i=pos;i<=bufSize-1;i++)
		desc[len++]=desc[i];
	desc[len]='\0';
}
int memcpy8           (unsigned char        * src,unsigned char        * desc,unsigned int buffSize                  ){
	unsigned int i=0;
	if(src==NULL||desc==NULL)
		return FALSE;
	while((i++)<buffSize)
		(*(desc++))=(*(src++));
	return TRUE;
}
int memcpy32          (unsigned int* src,unsigned int* desc,unsigned int buffSize                  ){
	unsigned int i=0;
	if(src==NULL||desc==NULL)
		return FALSE;
	while((i++)<buffSize)
		(*(desc++))=(*(src++));
	return TRUE;
}