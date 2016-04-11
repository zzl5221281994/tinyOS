#ifndef tiny_string_H
#define tiny_string_H
#include "tinyOS.h"
#define       isDigts(     X                      )     (((X)>='0'&&(X)<='9')?1:0                         )
#define      isLetter(     X                      )     ((((X)>='a'&&(X)<='z')||((X)>='A'&&(X)<='Z'))?1:0 )
#define isLowerLetter(     X                      )     (((X)>='a'&&(X)<='z')?1:0                         )
#define isUpperLetter(     X                      )     (((X)>='A'&&(X)<='Z')?1:0                         )
extern void intToStr         (u_int32  num,u_int8 *desc,    int32 bufSize					              );
extern u_int32 tiny_strlen   (int8*str                                                                    );
extern int32 memcpy8         (u_int8*  src,u_int8 *desc,  u_int32 buffSize                                );
extern int32 memcpy32        (u_int32* src,u_int32*desc,  u_int32 buffSize                                );
#endif