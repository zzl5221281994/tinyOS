#ifndef zzlOS.H
#define zzlOS.H
#define NULL 0
#define zzlOS_ERROR    0
#define zzlOS_SUCCESS  1
extern void intToStr           (unsigned int num,char*desc,int bufSize					                          );
extern int zzlOS_strlen        (char*str                                                                  );
extern int       isDigts       (char c                                                                    );
extern int       isLetter      (char c                                                                    );
extern int       isLowerLetter (char c                                                                    );
extern int       isUpperLetter (char c                                                                    );
#endif