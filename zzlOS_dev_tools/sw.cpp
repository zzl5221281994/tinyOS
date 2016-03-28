#include<stdio.h>
#include<string.h>
int strToInt (char*str                                          );
void intToStr(int num,char*desc  							    );
int main(int argc,char**argv){
	if(argc!=3)
	{
		printf("number of parameter error!\n");
		return 0;
	}
	char *path    =argv[1]             			;
	char resultStr[34]="times 512d db "		 	;
	int  sectorNum=strToInt(argv[2])			;
	FILE*fp  	  =fopen(path,"w+")			;
	for(int i=0;i<sectorNum;i++)
	{
		char desc[20];
		intToStr(i,desc);
		int pos=0;
		while(desc[pos]!='\0'){
			resultStr[14+pos]=desc[pos];
			pos++;
		}
		resultStr[14+pos]='\0';
		pos=0;
		while(resultStr[pos]!='\0'){
			putc(resultStr[pos],fp);
			pos++;
		}
		putc('\n',fp);
	}
	printf("succed!\n");
	fclose(fp);
	return 0;
}
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