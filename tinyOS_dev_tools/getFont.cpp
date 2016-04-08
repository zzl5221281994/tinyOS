#include<stdio.h>
#include<string>
#include<string.h>
using namespace std;
char*writeFile="result.txt";
char*readFile ="fontLib.txt";
int strToInt(char*str                                           );
void intToStr(int num,char*desc  							    );
int main(int argc,char**argv){
	if(argc!=2)
	{
		printf("number of parameter error!\n");
		return 0;
	}
	//char*arrayName=argv[1];
	//int start     =strToInt(argv[2]);
	int number    =strToInt(argv[1]);
	FILE*wf=fopen(writeFile,"w+");
	FILE*rf=fopen(readFile,"rb+");
	if(rf==NULL)
	{
		printf("EEEEE\n");
		return 0;
	}
	for(int key=0;key<number;key++)
	{
		char an[16][8];
		for(int i=0;i<16;i++)
		{
			for(int j=0;j<8;j++)
				an[i][j]=getc(rf);
			char current=getc(rf);
			current=getc(rf);
		}
		for(int i=0;i<16;i++)
		{
			for(int j=0;j<8;j++)
				printf("%c ",an[i][j]);
			printf("\n");
		}
		char desc[20];
		//intToStr(start,desc);
		//start++;
	    /*string number(desc);
		string result(arrayName);
		result+="[";
		result+=number;
		result+="]";*/
		int sum;
		for(int i=0;i<16;i++)
		{
			sum=0;
			for(int j=0;j<8;j++)
			{
				if(an[i][j]=='*')
					sum=(sum|(1<<(7-j)));
			}
			char tempStr[20];
			intToStr(sum,tempStr);
			string value(tempStr);
			
			string finalResult;
			finalResult=value+",";
			int len=finalResult.length();
			for(int k=0;k<len;k++)
				putc(finalResult[k],wf);
		}
    putc('\n',wf);		
	}
	fclose(rf);
	fclose(wf);
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