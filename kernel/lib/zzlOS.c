int isDigts       (char c                                        ){
	return (c>='0'&&c<='9')?1:0;
}
int isLowerLetter (char c                                        ){
	return (c>='a'&&c<='z')?1:0;
}
int isUpperLetter (char c                                        ){
	return (c>='A'&&c<='Z')?1:0;
}
int zzlOS_strlen  (char*str                                      ){
	int len=0;
	while((*str)!='\0')
	{
		str++;
		len++;
	}   
	return len;
}
void intToStr(int num,char*desc,int bufSize					     ){
	int pos=bufSize-1;
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
	int len=0,i;
	for(i=pos;i<=bufSize-1;i++)
		desc[len++]=desc[i];
	desc[len]='\0';
}