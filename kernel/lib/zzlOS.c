extern void write      (int i,char color);
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
	int i,j;
	//write(1024*30,0x1f);
		   
			  for(i=200;i<208;i++)
					for(j=200;j<216;j++)
						write(i*1024+j,0x31);
		   
	return len;
}