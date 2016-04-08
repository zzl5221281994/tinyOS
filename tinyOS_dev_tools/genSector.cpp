#include<string>
#include<stdio.h>
#include<iostream>
using namespace std;
string writeStr_SECTION="[SECTION .T";
string writeStr_BODY1  ="MOV		AX,0x0\nMOV		ES,AX\nMOV 	ax,0x7e19\nMOV 	bp,ax\nMOV 	cx,";
string writeStr_BODY2  ="MOV		ax,01301h\nMOV		bx,000ch\nMOV		dl,0\nint 10h\njmp     $\ndb \"read sector error!(C=0,H=0,S=";
string writeStr_TAIL   ="times	510d-($-$$) DB 0			\nDB		0x55, 0xaa";	
void intToStr(int num,char*desc  							    );
int strToInt(char*str                                           );
int main(int argc,char**argv){
	if(argc!=3)
	{
		printf("number of parameter error!\n");
		return 0;
	}
	char *path=argv[1];
	char *num =argv[2];
	FILE*fp=fopen(path,"w+");
	int numOf=strToInt(num);
	for(int i=1;i<=numOf;i++)
	{
		char desc[20];
		intToStr(i,desc);
		//int lengthStr=writeStr_SECTION.length();
		//int tempLen  =strlen(desc);
		string tempStr(desc,strlen(desc));
		///////////////////////////////////////////////
		string resultSection=writeStr_SECTION+tempStr+"]";
		string resultBODY2  =writeStr_BODY2+tempStr;
		resultBODY2.insert(resultBODY2.length(),1,')');
		resultBODY2.insert(resultBODY2.length(),1,'\"');
		//////////////////////////////////////////////
		intToStr(30+strlen(desc),desc);
		string tempStr1(desc,strlen(desc));
		string resultBODY1=writeStr_BODY1+tempStr1+"d";
		int tempLen=resultSection.length();
		for(int i=0;i<tempLen;i++)
			putc(resultSection[i],fp);
		putc('\n',fp);
		tempLen=resultBODY1.length();
		for(int i=0;i<tempLen;i++)
			putc(resultBODY1[i],fp);
		putc('\n',fp);
		tempLen=resultBODY2.length();
		for(int i=0;i<tempLen;i++)
			putc(resultBODY2[i],fp);
		putc('\n',fp);
		tempLen=writeStr_TAIL.length();
		for(int i=0;i<tempLen;i++)
			putc(writeStr_TAIL[i],fp);
		putc('\n',fp);
		putc('\n',fp);
		
	}
	fclose(fp);
	return 0;
}
/*#include<stdio.h>
#include<string.h>
int strToInt (char*str                                          );
int main(int argc,char**argv){
	if(argc!=3)
	{
		printf("number of parameter error!\n");
		return 0;
	}
	char *path    =argv[1]             			;
	char resultSectionStr[34]="times 512d db "		 	;
	int  sectorNum=strToInt(argv[2])			;
	FILE*fp  	  =fopen(path,"w+")			;
	for(int i=0;i<sectorNum;i++)
	{
		char desc[20];
		intToStr(i,desc);
		int pos=0;
		while(desc[pos]!='\0'){
			resultSectionStr[14+pos]=desc[pos];
			pos++;
		}
		resultSectionStr[14+pos]='\0';
		pos=0;
		while(resultSectionStr[pos]!='\0'){
			putc(resultSectionStr[pos],fp);
			pos++;
		}
		putc('\n',fp);
	}
	printf("succed!\n");
	fclose(fp);
	return 0;
}*/
int strToInt(char*str                                           ){
	int sum=0,strLen=strlen(str);
	int i;
	for(i=0;i<strLen;++i){
		sum=sum*10;
		sum=sum+str[i]-'0';
	}
	return sum;
}
void intToStr(int num,char*desc  							    ){
	int pos=19;
	if(num==0)
	{
		desc[0]='0';
		desc[1]='\0';
		return;
	}
	while(num!=0){
		int current=num%10;
		desc[pos]='0'+current;
		pos--;
		num=num/10;
	}
	pos++;
	int len=0;
	for(int i=pos;i<=19;i++)
		desc[len++]=desc[i];
	desc[len]='\0';
}