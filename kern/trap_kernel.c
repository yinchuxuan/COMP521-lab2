#ifndef _trap_kernel_c
#define _trap_kernel_c

#include "trap_kernel.h"
#include "pcb.h"
#include "pmem.h"
#include "common.h"
#include "load.h"

void trap_kernel_handler(ExceptionInfo* info) {
    int ret;

    switch (info->code) {
        case YALNIX_FORK:
            ret = yalnix_fork_handler();
            break;
        case YALNIX_EXEC:
            ret = yalnix_exec_handler((char*)info->regs[1], (char**)info->regs[2], info);
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
    /* create a child process */
    struct pcb* child_process = create_process();
    int vpn;
    struct physical_page_frame* page_frame;

    /* borrow one PTE in current page table at user_brk */
    int borrowed_PTE = VPN(UP_TO_PAGE(current_pcb->user_brk));

    /* copy process image in region0 from current process to child process(except kernel stack) */
    for (vpn = MEM_INVALID_PAGES; vpn < VPN(USER_STACK_LIMIT); vpn++) {
        if (current_pcb->region0_page_table[vpn].valid) {   // create physical page for child process at this page and copy content into it
            page_frame = allocate_page();
            if (!page_frame) {      // allocating physical memory failed
                exit(1); 
            }
            page_frame->page_reference++;

            child_process->region0_page_table[vpn].pfn = PFN(page_frame_to_physical_address(page_frame));
            child_process->region0_page_table[vpn].valid = VALID_PAGE;
            child_process->region0_page_table[vpn].kprot = current_pcb->region0_page_table[vpn].kprot;
            child_process->region0_page_table[vpn].uprot = current_pcb->region0_page_table[vpn].uprot;

            current_pcb->region0_page_table[borrowed_PTE].pfn = child_process->region0_page_table[vpn].pfn;
            current_pcb->region0_page_table[borrowed_PTE].valid = VALID_PAGE;
            current_pcb->region0_page_table[borrowed_PTE].kprot = PAGE_READ | PAGE_WRITE;

            /* copy content one page at a time */
            memcpy((void*)UP_TO_PAGE(current_pcb->user_brk), vpn * PAGESIZE, PAGESIZE);
            WriteRegister(REG_TLB_FLUSH, TLB_FLUSH_0);
        }
    }

    /* restore PTE in current page table */
    current_pcb->region0_page_table[borrowed_PTE].pfn = 0;
    current_pcb->region0_page_table[borrowed_PTE].valid = INVALID_PAGE;
    current_pcb->region0_page_table[borrowed_PTE].kprot = 0;

    WriteRegister(REG_TLB_FLUSH, TLB_FLUSH_0);

    /* save context for child process */
    ContextSwitch(switchFunction, &child_process->context, child_process, child_process);
    if (current_pcb->pid == child_process->pid) {    // return 0 for child process 
        return 0;
    }

    return child_process->pid;  // return pid for parent process
}

int yalnix_exec_handler(char* name, char** argvec, ExceptionInfo* info) {
    int ret = LoadProgram(name, argvec);

    if (ret == -1) {    // current process is still runnable
        return ERROR;
    }
    if (ret == -2) {    // current process is no longer runnable
        yalnix_exit_handler(1);
    }

    /* restore new context in exception info */
    int i;
    info->pc = current_pcb->pc;
    info->sp = current_pcb->sp;
    for (i = 0; i < NUM_REGS; i++) {
        info->regs[i] = current_pcb->regs[i]; 
    }
    info->psr = current_pcb->psr;
    current_pcb->status = RUNNING;

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
    if (addr > current_pcb->user_brk) {
        unsigned int new_brk = UP_TO_PAGE(addr);
        unsigned int address;
        int vpn;
        struct physical_page_frame* page_frame; 

        /* don't have enough memory */
        if (new_brk > current_pcb->user_stack_base - PAGESIZE) {
            printf("don't have enough memory to do brk!\n");
            return ERROR;
        }

        /* allocate memory between [user_brk, new_brk] */
        for (address = current_pcb->user_brk; address < new_brk; address += PAGESIZE) {
            vpn = VPN(address);

            if (!(page_frame = allocate_page())) {
                printf("don't have enough physical memory to do brk!\n");
                return ERROR;
            }
            page_frame->page_reference++;

            current_pcb->region0_page_table[vpn].pfn = PFN(page_frame_to_physical_address(page_frame));
            current_pcb->region0_page_table[vpn].valid = VALID_PAGE;
            current_pcb->region0_page_table[vpn].uprot = PAGE_READ | PAGE_WRITE;
            current_pcb->region0_page_table[vpn].kprot = PAGE_READ | PAGE_WRITE;
        }
    }
    
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