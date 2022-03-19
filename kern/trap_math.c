#ifndef _trap_math_c
#define _trap_math_c

#include <stdio.h>
#include "trap_math.h"
#include "trap_kernel.h"
#include "pcb.h"

void trap_math_handler(ExceptionInfo* info) {
    printf("trap math handler is called! process id: %d\n", current_pcb->pid);
    yalnix_exit_handler(1); 
}

#endif /* _trap_math_c */