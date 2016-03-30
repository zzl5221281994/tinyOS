extern void io_hlt(void);
extern void write(int i,char color);
char* str1="qwertyuiop";
char* str2="asdfghjkl";
struct rect{
	int leftX;
	int leftY;
	int lenX;
	int lenY;
	char color;
};
void HariMain(void)
{
	int i,j;
	//struct rect r1={100,100,50,50,0x1f};
	/*r1.leftX=100;
	r1.leftY=100;
	r1.lenX=50;
	r1.lenY=50;
	r1.color=0x1f;
	//write(r1.leftX*1024+r1.leftY,0x1f);*/
	for(i=50;i<100;i++)
		for(j=50;j<100;j++)
			write(1024*i+j,0x3c);
	/*for(i=r1.leftX;i<r1.leftX+r1.lenY;i++)
		for(j=r1.leftY;j<r1.leftY+r1.lenX;j++)
			write(1024*i+j,r1.color);*/
	
	while(1)
		io_hlt();
}
