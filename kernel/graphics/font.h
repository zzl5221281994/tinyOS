#ifndef FONT_H
#define FONT_H
#include "I:\work\tolset\tinyOS\kernel\lib\tinyOS.h   "
#define fontRect_WIDTH  8
#define fontRect_HEIGHT 16
extern void   drawStr  (u_int8 *str,    int32 leftX,int32 leftY,int32 foreGroundColor,int32 backGroundColor	                                         );
extern void   drawNum  (u_int32 num,    int32 leftX,int32 leftY,int32 foreGroundColor,int32 backGroundColor	                                         );
#endif
