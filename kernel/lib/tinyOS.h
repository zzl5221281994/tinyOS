#ifndef tinyOS_H
#define tinyOS_H
#define NULL 0
#define FALSE   0
#define TRUE    1
#define write_video8( add,color) ((*(vram8 +(add)))=(color))
#define write_video32(add,color) ((*(vram32+(add)))=(color))
extern char         *vram8 ;
extern unsigned int *vram32;
#endif
