/************************************************************************************
Wed Apr 13 15:38:40 2016

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
#ifndef interrupt_H
#define interrupt_H
#include "F:\work\tolset\tinyOS\kernel\lib\global.h       "
//CPU exception
extern u_int32 exception_hander[];
extern u_int32 interrupt_hander[];
extern u_int32 getErrorCode   (                    );
extern void    sys__IRQ_hander(                    );

extern void IRQ0_clock        (                    );
extern void IRQ1_keyBoard     (                    );
extern void IRQ2_slave        (                    );
extern void IRQ3_port2        (                    );
extern void IRQ4_port1        (                    );
extern void IRQ5_LPT2         (                    );
extern void IRQ6_floppyDisk   (                    );
extern void IRQ7_LPT1         (                    );

//slave
extern void IRQ8_CMOS         (                    );
extern void IRQ9_redirect_IRQ2(                    );
extern void IRQ10_reserved1   (                    );
extern void IRQ11_reserved2   (                    );
extern void IRQ12_PS2Mouse    (                    );
extern void IRQ13_FPU_error   (                    );
extern void IRQ14_ATDisk      (                    );
extern void IRQ15_reserved3   (                    );
#endif
