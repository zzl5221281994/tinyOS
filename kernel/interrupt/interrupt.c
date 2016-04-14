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
#include "F:\work\tolset\tinyOS\kernel\lib\global.h          "
#include "F:\work\tolset\tinyOS\kernel\debug\debug.h         "
#include "F:\work\tolset\tinyOS\kernel\bootInfo\bootInfo.h   "
#include "F:\work\tolset\tinyOS\kernel\kernelFun.h           "
#include "F:\work\tolset\tinyOS\kernel\graphics\font.h       "
#include "F:\work\tolset\tinyOS\kernel\IO\IO.h               "
#include "interrupt.h                                        "
#include "clock.h                                            "
#define EXCEPTION_HANDERS_NUM 20
#define INTERRUPT_HANDERS_NUM 16
#define DEFAULT_ERRORCODE     12345678
//CPU exception
PRIVATE void divide_exception               (void){
	sys_exception_panic(DEFAULT_ERRORCODE,0);
}
PRIVATE void debug_exception                (void){
	sys_exception_panic(DEFAULT_ERRORCODE,1);
}

PRIVATE void nMask_exception                (void){
	sys_exception_panic(DEFAULT_ERRORCODE,2);
}
PRIVATE void breakPoint_exception           (void){
	sys_exception_panic(DEFAULT_ERRORCODE,3);
}

PRIVATE void overflow_exception             (void){
	sys_exception_panic(DEFAULT_ERRORCODE,4);
}
PRIVATE void overBounds_exception           (void){
	sys_exception_panic(DEFAULT_ERRORCODE,5);
}
PRIVATE void invalid_op_exception           (void){
	sys_exception_panic(DEFAULT_ERRORCODE,6);
}
PRIVATE void device_not_available_exception (void){
	sys_exception_panic(DEFAULT_ERRORCODE,7);
}
PRIVATE void double_fault_exception         (void){
	u_int32 errorCode=getErrorCode();
	sys_exception_panic(errorCode,8);
}
PRIVATE void coprocessor_seg_over_exception (void){
	sys_exception_panic(DEFAULT_ERRORCODE,9);
}
PRIVATE void invalid_TSS_exception          (void){
	u_int32 errorCode=getErrorCode();
	sys_exception_panic(errorCode,10);
}
PRIVATE void seg_not_present_exception      (void){
	u_int32 errorCode=getErrorCode();
	sys_exception_panic(errorCode,11);
}
PRIVATE void stack_seg_exception            (void){
	u_int32 errorCode=getErrorCode();
	sys_exception_panic(errorCode,12);
}
PRIVATE void general_protection_exception   (void){
	u_int32 errorCode=getErrorCode();
	sys_exception_panic(errorCode,13);
}
PRIVATE void page_fault_exception           (void){
	u_int32 errorCode=getErrorCode();
	sys_exception_panic(errorCode,14);
}
PRIVATE void reserved_exception             (void){
	sys_exception_panic(DEFAULT_ERRORCODE,15);
}
PRIVATE void x87FPU_exception               (void){
	sys_exception_panic(DEFAULT_ERRORCODE,16);
}
PRIVATE void parallel_check_exception       (void){
	u_int32 errorCode=getErrorCode();
	sys_exception_panic(errorCode,17);
}
PRIVATE void machine_check_exception        (void){
	sys_exception_panic(DEFAULT_ERRORCODE,18);
}
PRIVATE void SIMD_exception                 (void){
	sys_exception_panic(DEFAULT_ERRORCODE,19);
}

//PIC interrupt
// master 8259A
PRIVATE void IRQ0_clock                     (void){
	close_interrupt();
	global_clock++;
	//drawNum(global_clock,boot_info.screen_height-16,boot_info.screen_width-80,0x1f,0x00);
	drawNum(global_clock,400,boot_info.screen_width-80,0x1f,0x00);
	sendEOI_Master ();
	open_interrupt();
	sys__IRQ_hander();
}
PRIVATE void IRQ1_keyBoard                  (void){
	close_interrupt();
	u_int8 byte=io_in8(0x60);
	keyBoard_inPut_buf[keyBoard_bufLen++]=byte;
	keyBoard_inPut_buf[keyBoard_bufLen]='\0';
	//drawStr(keyBoard_inPut_buf,600,0,0x1f,0x00);
	sendEOI_Master ();
	open_interrupt();
	sys__IRQ_hander();
}
PRIVATE void IRQ2_slave                     (void){
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ3_port2                     (void){
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ4_port1                     (void){
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ5_LPT2                      (void){
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ6_floppyDisk                (void){
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ7_LPT1                      (void){
	sendEOI_Master ();
	sys__IRQ_hander();
}
// slave 8259A
PRIVATE void IRQ8_CMOS                      (void){
	sendEOI_Slave  ();
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ9_redirect_IRQ2             (void){
	sendEOI_Slave  ();
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ10_reserved1                (void){
	sendEOI_Slave  ();
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ11_reserved2                (void){
	sendEOI_Slave  ();
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ12_PS2Mouse                 (void){
	sendEOI_Slave  ();
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ13_FPU_error                (void){
	sendEOI_Slave  ();
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ14_ATDisk                   (void){
	sendEOI_Slave  ();
	sendEOI_Master ();
	sys__IRQ_hander();
}
PRIVATE void IRQ15_reserved3                (void){
	sendEOI_Slave  ();
	sendEOI_Master ();
	sys__IRQ_hander();
}

//异常处理指针数组
PUBLIC u_int32 exception_hander[EXCEPTION_HANDERS_NUM]={
	                               (u_int32)divide_exception,
                                   (u_int32)debug_exception,
								   (u_int32)nMask_exception,
								   (u_int32)breakPoint_exception,
								   (u_int32)overflow_exception,
								   (u_int32)overBounds_exception,
								   (u_int32)invalid_op_exception,
								   (u_int32)device_not_available_exception,
								   (u_int32)double_fault_exception,
								   (u_int32)coprocessor_seg_over_exception,
								   (u_int32)invalid_TSS_exception,
								   (u_int32)seg_not_present_exception,
								   (u_int32)stack_seg_exception,
								   (u_int32)general_protection_exception,
								   (u_int32)page_fault_exception,
								   (u_int32)reserved_exception,
								   (u_int32)x87FPU_exception,
								   (u_int32)parallel_check_exception,
								   (u_int32)machine_check_exception,
								   (u_int32)SIMD_exception};
//中断处理指针数组
PUBLIC u_int32 interrupt_hander[INTERRUPT_HANDERS_NUM]={
	                                                   (u_int32)IRQ0_clock,
                                                       (u_int32)IRQ1_keyBoard,
													   (u_int32)IRQ2_slave,
													   (u_int32)IRQ3_port2,
													   (u_int32)IRQ4_port1,
													   (u_int32)IRQ5_LPT2,
													   (u_int32)IRQ6_floppyDisk,
													   (u_int32)IRQ7_LPT1,
													   (u_int32)IRQ8_CMOS,
													   (u_int32)IRQ9_redirect_IRQ2,
													   (u_int32)IRQ10_reserved1,
													   (u_int32)IRQ11_reserved2,
													   (u_int32)IRQ12_PS2Mouse,
													   (u_int32)IRQ13_FPU_error,
													   (u_int32)IRQ14_ATDisk,
													   (u_int32)IRQ15_reserved3};
													   