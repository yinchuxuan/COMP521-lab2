#ifndef _trap_memory_c
#define _trap_memory_c

#include <stdio.h>
#include <comp421/yalnix.h>
#include "trap_kernel.h"
#include "trap_clock.h"
#include "pcb.h"
#include "pmem.h"
#include "common.h"

void trap_memory_handler(ExceptionInfo* info) {
    printf("trap memory is called!\n");
    printf("address: %x\n", info->addr);

    if (info->addr > current_pcb->user_brk && info->addr < current_pcb->user_stack_base) {
        /* This is a request to enlarge user stack */
        unsigned int new_stack_base = DOWN_TO_PAGE(info->addr);
        unsigned int address;
        int vpn;
        struct physical_page_frame* page_frame;

        if (new_stack_base < current_pcb->user_brk + PAGESIZE) {
            printf("memory is not enough for enlarging user stack!\n");
            yalnix_exit_handler(1);
        }

        /* allocate memory between [new_stack_base, user_stack_base] */
        for (address = new_stack_base; address < current_pcb->user_stack_base; address += PAGESIZE) {
            vpn = VPN(address);

            if (!(page_frame = allocate_page())) {
                printf("don't have enough physical memory to enlarge user stack!\n");
                yalnix_exit_handler(1);
            }
            page_frame->page_reference++;

            current_pcb->region0_page_table[vpn].pfn = PFN(page_frame_to_physical_address(page_frame));
            current_pcb->region0_page_table[vpn].valid = VALID_PAGE;
            current_pcb->region0_page_table[vpn].uprot = PAGE_READ | PAGE_WRITE;
            current_pcb->region0_page_table[vpn].kprot = PAGE_READ | PAGE_WRITE;
        }

        return;
    }

    yalnix_exit_handler(1);
}

#endif /* _trap_memory_c */