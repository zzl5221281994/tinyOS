#define DEBUG(A) printf(#A"\n");
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char* ImgPath="test.img";
int writeBuf  (char*rst ,char* dst, int len, long from         );
int readFile  (char*path,char* buf, int len, long from         );
int writeFile (char*path,char* buf, int len, long from         );
int strToInt  (char*str                                        );
int main(int argc,char**argv)
{
	if(argc!=5){
		printf("number of parameter is wrong\n");
		return 0;
	}
    char*openFilePath  =argv[1];
	char*writeFrom_S   =argv[2];
	char*readFrom_S    =argv[3];
	char*writeLen_S    =argv[4];
	int write_from_i  =strToInt(writeFrom_S ),
	    read_from_i   =strToInt(readFrom_S  ),
	          len_i   =strToInt(writeLen_S  );
	char*fileBuff=(char*)malloc(sizeof(char)*len_i+1            );
	fileBuff[len_i]='\0';
	readFile (openFilePath,fileBuff,len_i,read_from_i           );
	writeFile(ImgPath,     fileBuff,len_i,write_from_i          );
	return 0;
}
int readFile(char*path,char* buf, int len, long from            ){
	FILE*fp=fopen(path,"rb+");
	if(fp==NULL){
		printf("readfile open error!\n");
		return 0;
	}
	fseek(fp,from,SEEK_SET);
	int i;
	for(i=0;i<len;++i)
		buf[i]=getc(fp);
	fclose(fp);
	return 1;
}
int writeFile(char*path,char* buf, int len, long from           ){
	FILE*fp=fopen(path,"rb+");
	if(fp==NULL){
		printf("writefile open error!\n");
		return 0;
	}
	fseek(fp,from,SEEK_SET);
	int i;
	for(i=0;i<len;++i)
		if((int)buf[i]==10)
		     putc(10,fp);
		else
			putc(buf[i],fp);
	fclose(fp);
	return 1;
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