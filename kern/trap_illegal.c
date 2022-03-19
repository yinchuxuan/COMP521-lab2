#ifndef _trap_illegal_c
#define _trap_illegal_c

#include <stdio.h>
#include "trap_illegal.h"
#include "trap_kernel.h"
#include "pcb.h"

void trap_illegal_handler(ExceptionInfo* info) {
    printf("trap illegal handler is called! process id: %d\n", current_pcb->pid);
    yalnix_exit_handler(1); 
}

#endif /* _trap_clock_c */