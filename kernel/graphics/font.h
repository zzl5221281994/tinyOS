/************************************************************************************
Wed Apr 13 15:36:06 2016

MIT License
Copyright (c) 2016 zhuzuolang

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************************************/
#ifndef FONT_H
#define FONT_H
#include "F:\work\tolset\tinyOS\kernel\lib\global.h            "
#define fontRect_WIDTH  8
#define fontRect_HEIGHT 16
extern void   drawStr  (u_int8 *str,    int32 leftX,int32 leftY,int32 foreGroundColor,int32 backGroundColor	                                         );
extern void   drawNum  (u_int32 num,    int32 leftX,int32 leftY,int32 foreGroundColor,int32 backGroundColor	                                         );
#endif
