/*-------------->x
/*|
/*|
/*|
/*|
/*|
/ \/
//Y
*/
#include"I:\work\tolset\zzlOS\kernel\lib\zzlOS.h"
#define type_NUMBER                     1
#define type_LOWER_LETTER               2
#define type_UPPER_LETTER               4
#define type_SIGN                       8
#define type_SPECIAL                    16
#define sign_NUMBER                     10
#define fontRect_WIDTH                  8
#define fontRect_HEIGHT                 16
static struct fontRect{
	unsigned char bitMap[16];
};
/*数字，大小写字母，符号，符号的查找表，以及一个特殊符号（表示错误字符）*/
static struct fontRect                          numberFont[10];
static struct fontRect                       charLowerFont[26];
static struct fontRect                       charUpperFont[26];
static struct fontRect                   signFont[sign_NUMBER];
static struct fontRect                             specialChar;
static char                         signFontTable[sign_NUMBER];
static void   drawFont(int type,	int leftX,int leftY,int foreGroundColor,int backGroundColor,int offset                                  );
       void   drawStr (char*str,    int leftX,int leftY,int foreGroundColor,int backGroundColor	                                            );
       void   drawNum (int num,     int leftX,int leftY,int foreGroundColor,int backGroundColor	                                            );
/////////////////////////////////////////////////
extern void write      (int i,char color);
extern int zzlOS_strlen(char*str        );
////////////////////////test
/*numberFont[0].bitMap[0]=0x00;
numberFont[0].bitMap[1]=0x18;
numberFont[0].bitMap[2]=0x24;
numberFont[0].bitMap[3]=0x24;
numberFont[0].bitMap[4]=0x42;
numberFont[0].bitMap[5]=0x42;
numberFont[0].bitMap[6]=0x42;
numberFont[0].bitMap[7]=0x42;
numberFont[0].bitMap[8]=0x42;
numberFont[0].bitMap[9]=0x42;
numberFont[0].bitMap[10]=0x42;
numberFont[0].bitMap[11]=0x24;
numberFont[0].bitMap[12]=0x24;
numberFont[0].bitMap[13]=0x18;
numberFont[0].bitMap[14]=0x00;
numberFont[0].bitMap[15]=0x00;*/

	   
static void   drawFont(int type,	int leftX,int leftY,int foreGroundColor,int backGroundColor,int offset                                  ){
	struct fontRect*fp=0;
	if(type&type_NUMBER)
		fp=&numberFont[offset];
	
	else if(type&type_LOWER_LETTER)
		fp=&charLowerFont[offset];
	
	else if(type&type_UPPER_LETTER)
		fp=&charUpperFont[offset];
	
	else if(type&type_SIGN        )
		fp=&signFont[offset];
	else
		fp=&specialChar;
	int i;
		for(i=0;i<16;++i)
		{
			unsigned char test=0x80,bit =(fp->bitMap)[i];
			int count=0,              startX=((leftX+i)<<10)+leftY;
			while(test>0)
			{
				if(test&bit)
					write(startX+count,foreGroundColor);
				else
					write(startX+count,backGroundColor);
				count++;
				test=(test>>1);
			}
		}
}
void   drawStr (char*str,    int leftX,int leftY,int foreGroundColor,int backGroundColor	                                            )
	   {
		   numberFont[0].bitMap[0]=0x00;
numberFont[0].bitMap[1]=0x18;
numberFont[0].bitMap[2]=0x24;
numberFont[0].bitMap[3]=0x24;
numberFont[0].bitMap[4]=0x42;
numberFont[0].bitMap[5]=0x42;
numberFont[0].bitMap[6]=0x42;
numberFont[0].bitMap[7]=0x42;
numberFont[0].bitMap[8]=0x42;
numberFont[0].bitMap[9]=0x42;
numberFont[0].bitMap[10]=0x42;
numberFont[0].bitMap[11]=0x24;
numberFont[0].bitMap[12]=0x24;
numberFont[0].bitMap[13]=0x18;
numberFont[0].bitMap[14]=0x00;
numberFont[0].bitMap[15]=0x00;
				int i,j;
				for(i=200;i<208;i++)
					for(j=200;j<216;j++)
						write(i*1024+j,0x2e);
		   int len=zzlOS_strlen(str);
		   int startX=leftX,startY=leftY;
		   if(len==2)
		   {
			   for(i=100;i<108;i++)
					for(j=100;j<116;j++)
						write(i*1024+j,0x3c);
		   }
		   for(i=300;i<308;i++)
					for(j=300;j<316;j++)
						write(i*1024+j,0x1f);
		   for(i=0;i<len;++i)
		   {
			   if(isDigts(str[i]))
				   drawFont(type_NUMBER,        startX,startY, foreGroundColor, backGroundColor, str[i]-'0');
			   
		       else if(isLowerLetter(str[i]))
				   drawFont(type_LOWER_LETTER,  startX,startY, foreGroundColor, backGroundColor, str[i]-'a');
			   
		       else if(isUpperLetter(str[i]))
				   drawFont(type_UPPER_LETTER,  startX,startY, foreGroundColor, backGroundColor, str[i]-'A');
			   
		       else
			   {
				   int off=-1;
				   int j;
				   for(j=0;j<sign_NUMBER;++j)
					   if(str[i]==signFontTable[j])
					   {
						   off=j;
						   break;
					   }
				   drawFont(type_SIGN,          startX,startY, foreGroundColor, backGroundColor,off        );
			   }
			   startY+=fontRect_WIDTH;
			   if(startY>=1024)
			   {
				   startX+=fontRect_HEIGHT;
				   startY=0;
			   }
		   }
	   }
void   drawNum (int num,     int leftX,int leftY,int foreGroundColor,int backGroundColor	                                            ){
	   }

	   
	   
	   
	   
	   
	   
	   
	   
	   
	   
	   
	   
	   
	   
	   
	   
	   
	   