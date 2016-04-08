#include<stdio.h>
int main(int argc,char**argv){
	if(argc!=2)
	{
		printf("number of parameter error!\n");
		return 0;
	}
	char *path=argv[1];
	FILE*fp=fopen(path,"w+");
	fclose(fp);
	return 0;
}