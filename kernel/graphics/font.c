//* -------------->x
//* |
//* |
//* |
//* |
//* |
//  \/
// Y

#include "H:\work\tolset\tinyOS\kernel\lib\tiny_string.h"
#include "H:\work\tolset\tinyOS\kernel\lib\tinyOS.h"
#define type_NUMBER                     1
#define type_LOWER_LETTER               2
#define type_UPPER_LETTER               4
#define type_SIGN                       8
#define type_SPECIAL                    16
#define sign_NUMBER                     10
#define fontRect_WIDTH                  8
#define fontRect_HEIGHT                 16
/*数字，大小写字母，符号，符号的查找表，以及一个特殊符号（表示错误字符）*/
unsigned char                          numberFont[10][16]={0,24,36,36,66,66,66,66,66,66,66,36,36,24,0,0,
                                                           0,8,24,40,8,8,8,8,8,8,8,8,8,62,0,0,
                                                           0,24,36,66,66,2,4,8,16,32,32,64,64,126,0,0,
                                                           0,24,36,66,2,2,4,24,4,2,2,66,36,24,0,0,
                                                           0,12,12,12,20,20,20,36,36,68,126,4,4,30,0,0,
                                                           0,124,64,64,64,88,100,2,2,2,2,66,36,24,0,0,
                                                           0,24,36,66,64,88,100,66,66,66,66,66,36,24,0,0,
                                                           0,126,66,66,4,4,8,8,8,16,16,16,16,56,0,0,
                                                           0,24,36,66,66,66,36,24,36,66,66,66,36,24,0,0,
                                                           0,24,36,66,66,66,66,66,38,26,2,66,36,24,0,0};
unsigned char                       charLowerFont[26][16]={0,0,0,0,0,112,8,4,60,68,132,132,140,118,0,0,
                                                           192,64,64,64,64,88,100,66,66,66,66,66,100,88,0,0,
                                                           0,0,0,0,0,48,76,132,132,128,128,130,68,56,0,0,
                                                           12,4,4,4,4,52,76,132,132,132,132,132,76,54,0,0,
                                                           0,0,0,0,0,56,68,130,130,252,128,130,66,60,0,0,
                                                           14,16,16,16,16,124,16,16,16,16,16,16,16,124,0,0,
                                                           0,0,0,0,0,54,76,132,132,132,132,76,52,4,4,120,
                                                           192,64,64,64,64,88,100,66,66,66,66,66,66,227,0,0,
                                                           0,16,16,0,0,48,16,16,16,16,16,16,16,56,0,0,
                                                           0,4,4,0,0,12,4,4,4,4,4,4,4,8,8,48,
                                                           192,64,64,64,64,78,68,72,80,96,80,72,68,230,0,0,
                                                           48,16,16,16,16,16,16,16,16,16,16,16,16,56,0,0,
                                                           0,0,0,0,0,246,73,73,73,73,73,73,73,219,0,0,
                                                           0,0,0,0,0,216,100,66,66,66,66,66,66,227,0,0,
                                                           0,0,0,0,0,56,68,130,130,130,130,130,68,56,0,0,
                                                           0,0,0,0,0,216,100,66,66,66,66,66,100,88,64,224,
                                                           0,0,0,0,0,52,76,132,132,132,132,132,76,52,4,14,
                                                           0,0,0,0,0,220,98,66,64,64,64,64,64,224,0,0,
                                                           0,0,0,0,0,122,134,130,192,56,6,130,194,188,0,0,
                                                           0,0,16,16,16,124,16,16,16,16,16,16,16,14,0,0,
                                                           0,0,0,0,0,198,66,66,66,66,66,66,70,59,0,0,
                                                           0,0,0,0,0,231,66,66,66,36,36,36,24,24,0,0,
                                                           0,0,0,0,0,231,66,66,90,90,90,36,36,36,0,0,
                                                           0,0,0,0,0,198,68,40,40,16,40,40,68,198,0,0,
                                                           0,0,0,0,0,231,66,66,36,36,36,24,24,16,16,96,
                                                           0,0,0,0,0,254,130,132,8,16,32,66,130,254,0,0};
unsigned char                       charUpperFont[26][16]={0,24,24,24,24,36,36,36,36,126,66,66,66,231,0,0,
                                                           0,240,72,68,68,68,72,120,68,66,66,66,68,248,0,0,
                                                           0,58,70,66,130,128,128,128,128,128,130,66,68,56,0,0,
                                                           0,248,68,68,66,66,66,66,66,66,66,68,68,248,0,0,
                                                           0,254,66,66,64,64,68,124,68,64,64,66,66,254,0,0,
                                                           0,254,66,66,64,64,68,124,68,68,64,64,64,240,0,0,
                                                           0,58,70,66,130,128,128,158,130,130,130,66,70,56,0,0,
                                                           0,231,66,66,66,66,66,126,66,66,66,66,66,231,0,0,
                                                           0,124,16,16,16,16,16,16,16,16,16,16,16,124,0,0,
                                                           0,31,4,4,4,4,4,4,4,4,4,4,132,72,48,0,
                                                           0,231,66,68,72,80,80,96,80,80,72,68,66,231,0,0,
                                                           0,240,64,64,64,64,64,64,64,64,64,66,66,254,0,0,
                                                           0,195,66,102,102,102,90,90,90,66,66,66,66,231,0,0,
                                                           0,199,66,98,98,82,82,82,74,74,74,70,70,226,0,0,
                                                           0,56,68,130,130,130,130,130,130,130,130,130,68,56,0,0,
                                                           0,248,68,66,66,66,68,120,64,64,64,64,64,240,0,0,
                                                           0,56,68,130,130,130,130,130,130,130,146,138,68,58,0,0,
                                                           0,252,66,66,66,66,124,68,66,66,66,66,66,231,0,0,
                                                           0,58,70,130,130,128,64,56,4,2,130,130,196,184,0,0,
                                                           0,254,146,146,16,16,16,16,16,16,16,16,16,124,0,0,
                                                           0,231,66,66,66,66,66,66,66,66,66,66,36,60,0,0,
                                                           0,231,66,66,66,66,36,36,36,36,24,24,24,24,0,0,
                                                           0,231,66,66,66,90,90,90,90,36,36,36,36,36,0,0,
                                                           0,231,66,66,36,36,36,24,36,36,36,66,66,231,0,0,
                                                           0,238,68,68,68,40,40,40,16,16,16,16,16,124,0,0,
                                                           0,254,132,132,8,8,16,16,32,32,64,66,130,254,0,0};
unsigned char                   signFont[sign_NUMBER][16];
unsigned char                             specialChar;
static char                         signFontTable[sign_NUMBER];
static void   drawFont (int type,	int leftX,int leftY,int foreGroundColor,int backGroundColor,int offset                                      );
       void   init_font(                                                                                                                        );
       void   drawStr  (char*str,    int leftX,int leftY,int foreGroundColor,int backGroundColor	                                            );
       void   drawNum  (unsigned int num,     int leftX,int leftY,int foreGroundColor,int backGroundColor	                                            );
	 
/////////////////////////////////////////////////
extern int tiny_strlen(char*str        );



static void   drawFont(int type,	int leftX,int leftY,int foreGroundColor,int backGroundColor,int offset                                  ){
	unsigned char*fp=NULL;
	if(type&type_NUMBER)
		fp=&numberFont[offset][0];
	
	else if(type&type_LOWER_LETTER)
		fp=&charLowerFont[offset][0];
	
	else if(type&type_UPPER_LETTER)
		fp=&charUpperFont[offset][0];
	
	else if(type&type_SIGN        )
		fp=&signFont[offset][0];
	else
		fp=&specialChar;
	int i;
		for(i=0;i<16;++i)
		{
			unsigned char test=0x80,bit =fp[i];
			int count=0,              startX=((leftX+i)<<10)+leftY;
			while(test>0)
			{
				if(test&bit)
					write_video8(startX+count,foreGroundColor);
				else
					write_video8(startX+count,backGroundColor);
				count++;
				test=(test>>1);
			}
		}
}
void   drawStr (char*str,    int leftX,int leftY,int foreGroundColor,int backGroundColor	                                                )
	   {
				int i,j;
		   int len=tiny_strlen(str);
		   int startX=leftX,startY=leftY;
		   for(i=0;i<len;++i)
		   {
			   if(isDigts(str[i]))
				   drawFont(type_NUMBER,        startX,startY, foreGroundColor, backGroundColor, str[i]-'0');

		       else if(isLowerLetter(str[i]))
				   drawFont(type_LOWER_LETTER,  startX,startY, foreGroundColor, backGroundColor, str[i]-'a');
			   
		       else if(isUpperLetter(str[i]))
				   drawFont(type_UPPER_LETTER,  startX,startY, foreGroundColor, backGroundColor, str[i]-'A');
			   /////////////////////////////////////////////test
			   else if(str[i]==' ')
			   {
				   startY+=fontRect_WIDTH;
				   if(startY>=1024)
			          {
						  startX+=fontRect_HEIGHT;
						  startY=0;
			          }
			   continue;
			   }
			   else if(str[i]=='\n')
			   {
				   startY=0;
				   startX+=fontRect_HEIGHT;
				   continue;
			   }
			   /////////////////////////////////////////////test
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
	   
void   drawNum (unsigned int num,     int leftX,int leftY,int foreGroundColor,int backGroundColor	                                                ){
	char desc[20];
	intToStr(num,desc,20);
	drawStr(desc,leftX,leftY,foreGroundColor,backGroundColor);
    }
