/************************************************************************************
Wed Apr 13 15:37:58 2016

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
#ifndef kernelFun_H
#define kernelFun_H
#include "lib\global.h"
extern void    io_hlt                (                                         );
extern void    setGdt                (u_int8*lgdt                              );
extern void    setIdt                (u_int8*lidt                              );
extern u_int8  io_in8                (u_int32 port                             );
extern u_int16 io_in16               (u_int32 port                             );
extern u_int32 io_in32               (u_int32 port                             );
extern void    io_out8               (u_int32 port,u_int32 data                );
extern void    io_out16              (u_int32 port,u_int32 data                );
extern void    io_out32              (u_int32 port,u_int32 data                );
extern void    io_delay              (                                         );
extern void    port_read             (u_int16 port, void* buf, int32 n         );
extern void    port_write            (u_int16 port, void* buf, int32 n         );
extern void    load_master_maskWord  (u_int8 word                              );
extern void    load_slave_maskWord   (u_int8 word                              );
//extern void    sendEOI_Master        (                                         );
//extern void    sendEOI_Slave         (                                         );
extern void    open_interrupt        (                                         );
extern void    close_interrupt       (                                         );
extern void    loadTss               (                                         );
extern void    loadLdt               (                                         );
extern void    loadReg               (u_int32 eip,u_int32 cs,u_int32 esp,u_int32 ss,u_int32 eax,  u_int32 ecx,u_int32 edx,u_int32 ebx,u_int32 ebp,u_int32 esi,u_int32 edi,u_int32 eflags);
#endif
