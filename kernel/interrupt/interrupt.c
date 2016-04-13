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
//CPU exception
void divide_exception               (void);    // int 0
void debug_exception                (void);    // int 1

void nMask_exception                (void);    // int 2
void breakPoint_exception           (void);    // int 3

void overflow_exception             (void);    // int 4
void overBounds_exception           (void);    // int 5
void invalid_op_exception           (void);    // int 6
void device_not_available_exception (void);    // int 7
void double_fault_exception         (void);    // int 8
void coprocessor_seg_over_exception (void);    // int 9
void invalid_TSS_exception          (void);    // int 10
void seg_not_present_exception      (void);    // int 11
void stack_seg_exception            (void);    // int 12
void general_protection_exception   (void);    // int 13
void page_fault_exception           (void);    // int 14
void reserved_exception             (void);    // int 15
void x87FPU_exception               (void);    // int 16
void parallel_check_exception       (void);    // int 17
void machine_check_exception        (void);    // int 18
void SIMD_exception                 (void);    // int 19

//PIC interrupt
// master 8259A
void IRQ0_time                      (void);    // int 0x20
void IRQ1_keyBoard                  (void);    // int 0x21
void IRQ3_port2                     (void);    // int 0x22
void IRQ4_port1                     (void);    // int 0x23
void IRQ5_LPT2                      (void);    // int 0x24
void IRQ6_floppyDisk                (void);    // int 0x25
void IRQ7_LPT1                      (void);    // int 0x26
// slave 8259A
void IRQ8_CMOS                      (void);    // int 0x28
void IRQ9_redirect_IRQ2             (void);    // int 0x29
void IRQ10_reserved1                (void);    // int 0x2a
void IRQ11_reserved2                (void);    // int 0x2b
void IRQ12_PS2Mouse                 (void);    // int 0x2c
void IRQ13_FPU_error                (void);    // int 0x2d
void IRQ14_ATDisk                   (void);    // int 0x2e
void IRQ15_reserved3                (void);    // int 0x2f