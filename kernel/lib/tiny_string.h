#ifndef tiny_string_H
#define tiny_string_H
#define       isDigts(     X                      )     (((X)>='0'&&(X)<='9')?1:0                         )
#define      isLetter(     X                      )     ((((X)>='a'&&(X)<='z')||((X)>='A'&&(X)<='Z'))?1:0 )
#define isLowerLetter(     X                      )     (((X)>='a'&&(X)<='z')?1:0                         )
#define isUpperLetter(     X                      )     (((X)>='A'&&(X)<='Z')?1:0                         )
extern void intToStr         (unsigned int num,unsigned char*desc,int bufSize					                   );
extern int tiny_strlen       (unsigned char*str                                                                    );
extern int memcpy8           (unsigned char        * src,unsigned char        * desc,unsigned int buffSize         );
extern int memcpy32          (unsigned int* src,unsigned int* desc,unsigned int buffSize                           );
#endif