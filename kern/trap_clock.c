#ifndef _trap_clock_c
#define _trap_clock_c

#include "trap_clock.h"
#include "pcb.h"

void trap_clock_handler(ExceptionInfo* info) {
    struct delayed_pcb* tmp = delayed_pcb_list; 
    printf("trap clock handler is called!\n");
    
    while (tmp->next != NULL) {    // update delayed pcb blocks
        if (--tmp->next->remaining_clock_ticks == 0) {      // unblock process, remove it from delayed list
            tmp->next->process->status = READY;
            struct delayed_pcb* restore = tmp->next;
            tmp->next = tmp->next->next; 
            free(restore);
            continue;
        }

       tmp = tmp->next; 
    }

    if (++current_pcb->clock_ticks == MAX_CLOCK_TICKS) {      // should switch process now
        current_pcb->clock_ticks = 0;
        current_pcb->status = READY;
        round_robin_process_schedule();
    }
}

#endif /* _trap_clock_c */