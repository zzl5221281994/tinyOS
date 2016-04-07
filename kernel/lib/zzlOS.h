#ifndef zzlOS_H
#define zzlOS_H
#define NULL 0
#define FALSE   0
#define TRUE    1
#define write_video8( add,color) ((*(vram8 +(add)))=(color))
#define write_video32(add,color) ((*(vram32+(add)))=(color))
#define           min(         X      ,Y          )                      \
                                        ({                     \
                                           typeof (X) x_ = (X);\  
                                           typeof (Y) y_ = (Y);\  
                                           (x_ < y_) ? x_ : y_;\
			                            })
#define           max(         X      ,Y          )                      \
                                        ({                     \
                                           typeof (X) x_ = (X);\  
                                           typeof (Y) y_ = (Y);\  
                                           (x_ > y_) ? x_ : y_;\
			                            })
#define       isDigts(     X                      )     (((X)>='0'&&(X)<='9')?1:0                         )
#define      isLetter(     X                      )     ((((X)>='a'&&(X)<='z')||((X)>='A'&&(X)<='Z'))?1:0 )
#define isLowerLetter(     X                      )     (((X)>='a'&&(X)<='z')?1:0                         )
#define isUpperLetter(     X                      )     (((X)>='A'&&(X)<='Z')?1:0                         )
extern void intToStr           (unsigned int num,char*desc,int bufSize					                  );
extern int zzlOS_strlen        (char*str                                                                  );
extern char         *vram8 ;
extern unsigned int *vram32;
#endif