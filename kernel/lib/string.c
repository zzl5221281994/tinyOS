/************************************************************************************
Wed Apr 13 15:37:28 2016

MIT License
Copyright (c) 2016 zhuzuolang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************************/
#include "global.h"
PUBLIC u_int32 strlen  (int8*str                                                   ){
	int32 len=0;
	while((*str)!='\0')
	{
		str++;
		len++;
	}   
	return len;
}
PUBLIC u_int32 strcmp        (int8*src,int8*desc                                   ){
	while(*(src)==*(desc))
	{
		if(*src=='\0')
			return TRUE;
		src++;
		desc++;
	}
	return FALSE;
}
PUBLIC void intToStr        (u_int32 num ,u_int8*  desc ,int32   bufSize				 ){
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
PUBLIC int32 memcpy8        (u_int8*  src,u_int8*  desc ,u_int32 buffSize               ){
	u_int32 i=0;
	if(src==NULL||desc==NULL)
		return FALSE;
	while((i++)<buffSize)
		(*(desc++))=(*(src++));
	return TRUE;
}
PUBLIC int32 memcpy32       (u_int32* src,u_int32* desc ,u_int32 buffSize               ){
	u_int32 i=0;
	if(src==NULL||desc==NULL)
		return FALSE;
	while((i++)<buffSize)
		(*(desc++))=(*(src++));
	return TRUE;
}
