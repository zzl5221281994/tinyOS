#include<iostream>
#include<time.h>
#include<string>
#include<stdio.h>
#include<string.h>
using namespace std;
string licenseStr="MIT License\nCopyright (c) 2016 zhuzuolang\n\nPermission is hereby granted, free of charge, to any person obtaining a copy\nof this software and associated documentation files (the \"Software\"), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and/or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions:\nThe above copyright notice and this permission notice shall be included in all\ncopies or substantial portions of the Software.\nTHE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.\n";
string    headStr="/************************************************************************************\n";
string    tailStr="************************************************************************************/\n";
char readBuff[1000000];
int  readBuffLen;
int main(int argc,char**argv){
	if(argc==1)
	{
		printf("number of parameter error!\n");
		return 0;
	}
	time_t timer=time(NULL);  
    struct tm *tblock=localtime(&timer);
	string timeStr(asctime(tblock));
	timeStr.insert(timeStr.length(),1,'\n');
	for(int i=1;i<argc;i++)
	{
		char*path=argv[i];
		int pathLen=strlen(path);
		readBuffLen=0;
		FILE*fp=fopen(path,"rb+");
		while((readBuff[readBuffLen]=getc(fp))!=EOF)
			readBuffLen++;
		string tempStr(readBuff,readBuffLen);
		string resultStr=headStr+timeStr+licenseStr+tailStr;
		if((path[pathLen-1]=='s'&&path[pathLen-2]=='a'&&path[pathLen-3]=='n')||(path[pathLen-1]=='c'&&path[pathLen-2]=='n'&&path[pathLen-3]=='i'))
		{
			resultStr.insert(0,1,';');
			int tempLen=1;
			while(tempLen<resultStr.size())
			{
				if(resultStr[tempLen]=='\n')
					resultStr.insert(tempLen+1,1,';');
				tempLen++;
			}
		}
		resultStr=resultStr.substr(0,resultStr.length()-1);
		resultStr=resultStr+tempStr;
		//////////////////////////////
		fclose(fp);
		fopen(path,"w+");
		fclose(fp);
		fopen(path,"rb+");
		/////////////////////////////
		fseek(fp,0,SEEK_SET);
		int resultLen=resultStr.length();
		for(int j=0;j<resultLen;j++)
			putc(resultStr[j],fp);
	fclose(fp);
	}
	return 0;
}
