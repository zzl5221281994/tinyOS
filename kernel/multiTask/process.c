#include "process.h"
PUBLIC struct proc_table process_table[MAX_PROCESS];
PUBLIC struct TSS global_tss;
PUBLIC u_int32 current_proc_pid=0;
PUBLIC void schedule();
void init_tss();