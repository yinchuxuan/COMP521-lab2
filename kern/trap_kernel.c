#ifndef _trap_kernel_c
#define _trap_kernel_c

#include "trap_kernel.h"
#include "pcb.h"

void trap_kernel_handler(ExceptionInfo* info) {
    int ret;

    switch (info->code) {
        case YALNIX_FORK:
            ret = yalnix_fork_handler();
            break;
        case YALNIX_EXEC:
            ret = yalnix_exec_handler((char*)info->regs[1], (char**)info->regs[1]);
            break;
        case YALNIX_EXIT:
            ret = yalnix_exit_handler((int)info->regs[1]);
            break;
        case YALNIX_WAIT:
            ret = yalnix_wait_handler((int*)info->regs[1]);
            break;
        case YALNIX_GETPID:
            ret = yalnix_getpid_handler();
            break;
        case YALNIX_BRK:
            ret = yalnix_brk_handler((void*)info->regs[1]);
            break;
        case YALNIX_DELAY:
            ret = yalnix_delay_handler((int)info->regs[1]);
            break;
        case YALNIX_TTY_READ:
            ret = yalnix_tty_read_handler((int)info->regs[1], (void*)info->regs[2], (int)info->regs[3]);
            break;
        case YALNIX_TTY_WRITE:
            ret = yalnix_tty_write_handler((int)info->regs[1], (void*)info->regs[2], (int)info->regs[3]);
            break;
        default:
            return ERROR;
    }

    info->regs[0] = (unsigned long)ret;
}

int yalnix_fork_handler() {
    return 0;
}

int yalnix_exec_handler(char* name, char** argvec) {
    return 0;
}

int yalnix_exit_handler(int status) {
    /* lazy deleteion for pcb */
    current_pcb->status = TERMINATED;
    round_robin_process_schedule();

    /* should never get there */
    return 0;
}

int yalnix_wait_handler(int *status_ptr) {
    return 0;
}

int yalnix_getpid_handler() {
    return current_pcb->pid; 
}

int yalnix_brk_handler(void* addr) {
    return 0;
}

int yalnix_delay_handler(int clock_ticks) {
    if (clock_ticks < 0) {   // clock ticks should > 0
        return ERROR;
    }

    if (clock_ticks > 0) {   // add a delayed pcb block to delayed pcb queue
        struct delayed_pcb* new_delayed_pcb = (struct delayed_pcb*)malloc(sizeof(struct delayed_pcb)); 
        if (!new_delayed_pcb) {
            printf("Can't allocate memory for delayed pcb block!\n");
            return ERROR;
        }

        new_delayed_pcb->process = current_pcb;
        new_delayed_pcb->remaining_clock_ticks = clock_ticks;
        new_delayed_pcb->next = delayed_pcb_list->next;
        delayed_pcb_list->next = new_delayed_pcb;
        current_pcb->status = WAITING;
        round_robin_process_schedule();
    }

    return 0;
}

int yalnix_tty_read_handler(int tty_id, void* buf, int len) {
    return 0;
}

int yalnix_tty_write_handler(int tty_id, void* buf, int len) {
    return 0;
}


#endif /* _trap_kernel_c */