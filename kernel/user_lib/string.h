/************************************************************************************
Wed Apr 13 15:37:28 2016

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
#ifndef string_H
#define string_H
#include "global.h"
#define       isDigts(     X                      )     (((X)>='0'&&(X)<='9')?1:0                         )
#define      isLetter(     X                      )     ((((X)>='a'&&(X)<='z')||((X)>='A'&&(X)<='Z'))?1:0 )
#define isLowerLetter(     X                      )     (((X)>='a'&&(X)<='z')?1:0                         )
#define isUpperLetter(     X                      )     (((X)>='A'&&(X)<='Z')?1:0                         )
extern void intToStr         (u_int32  num,u_int8 *desc,    int32 bufSize					              );
extern u_int32 strlen        (int8*str                                                                    );
extern int32 memcpy8         (u_int8*  src,u_int8 *desc,  u_int32 buffSize                                );
extern int32 memcpy32        (u_int32* src,u_int32*desc,  u_int32 buffSize                                );
#endif
