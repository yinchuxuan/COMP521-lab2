#ifndef _pcb_c
#define _pcb_c

#include <string.h>
#include "kbrk.h"
#include "boot.h"
#include "pmem.h"
#include "common.h"
#include "pcb.h"
#include "boot.h"

unsigned int next_pid = 2;

SavedContext* switchFunction(SavedContext* old_context, void* old_pcb, void* new_pcb) {
    struct pcb* old_pcb_p = (struct pcb*)old_pcb;
    struct pcb* new_pcb_p = (struct pcb*)new_pcb;

    /* Save current context */
    old_pcb_p->context = *old_context;

    /* just for saving context and copying kernel stack */
    if (old_pcb_p->pid == new_pcb_p->pid) {
        /* borrow one PTE in current page table at user_brk */
        int borrowed_PTE = VPN(UP_TO_PAGE(current_pcb->user_brk));
        int i;
        for (i = 0; i < KERNEL_STACK_PAGES; i++) {
            current_pcb->region0_page_table[borrowed_PTE].pfn = old_pcb_p->region0_page_table[VPN(KERNEL_STACK_BASE) + i].pfn;
            current_pcb->region0_page_table[borrowed_PTE].valid = VALID_PAGE;
            current_pcb->region0_page_table[borrowed_PTE].kprot = PAGE_READ | PAGE_WRITE;

            /* copy current kernel stack to physical address space of new pcb's kernel stack one page at a time */
            memcpy((void*)UP_TO_PAGE(current_pcb->user_brk), KERNEL_STACK_BASE + i * PAGESIZE, PAGESIZE);
            WriteRegister(REG_TLB_FLUSH, TLB_FLUSH_0);
        }

        /* restore PTE in current page table */
        current_pcb->region0_page_table[borrowed_PTE].pfn = 0;
        current_pcb->region0_page_table[borrowed_PTE].valid = INVALID_PAGE;
        current_pcb->region0_page_table[borrowed_PTE].kprot = 0;

        WriteRegister(REG_TLB_FLUSH, TLB_FLUSH_0);

        return &old_pcb_p->context;
    }

    /* switch region0 table page */
    WriteRegister(REG_PTR0, (RCS421RegVal)new_pcb_p->region0_page_table);
    WriteRegister(REG_TLB_FLUSH, TLB_FLUSH_0);

    return &new_pcb_p->context;
}

struct pcb* create_process() {
    struct pcb* new_pcb = (struct pcb*)malloc(sizeof(struct pcb));
    new_pcb->next = pcb_list->next;
    pcb_list->next = new_pcb;
    new_pcb->pid = next_pid++;
    new_pcb->clock_ticks = 0;
    new_pcb->status = READY;
    new_pcb->user_brk = MEM_INVALID_SIZE;
    new_pcb->user_stack_base = USER_STACK_LIMIT;
    new_pcb->region0_page_table = (struct pte*)malloc(PAGE_TABLE_SIZE);
    if (!new_pcb->region0_page_table) {
        // deal with allocation failure
    }

    setup_region0_page_table(new_pcb->region0_page_table);
    return new_pcb;
}

void terminate_process(struct pcb* process) {
    unsigned int address;
    int vpn;
    struct physical_page_frame* page_frame;

    /* don't delete init process's page table, since this is initial region0 page table */
    if (process->pid == 1) {
        /* unload init program */
        for(address = 0; address < VMEM_0_LIMIT; address += PAGESIZE) {
            if (address < DOWN_TO_PAGE(KERNEL_STACK_BASE) || address >= UP_TO_PAGE(KERNEL_STACK_LIMIT)) {
                vpn = VPN(address);

                /* should deallocate physical page */
                if (process->region0_page_table[vpn].valid)  {
                    page_frame = physical_address_to_page_frame(process->region0_page_table[vpn].pfn << PAGESHIFT);
                    if ((--page_frame->page_reference) == 0) {
                        free_page(page_frame);
                    }            
                }
            }
        }
        
        WriteRegister(REG_TLB_FLUSH, TLB_FLUSH_0);
    } else {
        for(address = 0; address < VMEM_0_LIMIT; address += PAGESIZE) {
            vpn = VPN(address);

            /* should deallocate physical page */
            if (process->region0_page_table[vpn].valid)  {
                page_frame = physical_address_to_page_frame(process->region0_page_table[vpn].pfn << PAGESHIFT);
                if ((--page_frame->page_reference) == 0) {
                    free_page(page_frame);
                }            
            }
        }

        /* deallocate page table */
        free(process->region0_page_table);
    }
}

void round_robin_process_schedule() {
    struct pcb* tmp = pcb_list;
    struct pcb* old_pcb = current_pcb; 

    /* try to find runnable process, switch to it */
    while (tmp->next != NULL) {
        if (tmp->next->status == TERMINATED) {   // lazy deleteion
            struct pcb* delete_pcb = tmp->next;
            tmp->next = delete_pcb->next;
            terminate_process(delete_pcb);
            free(delete_pcb);
            continue; 
        }

        if (tmp->next->status == READY && tmp->next->pid != old_pcb->pid) {
            tmp->next->status = RUNNING;
            current_pcb = tmp->next;
            ContextSwitch(&switchFunction, &old_pcb->context, old_pcb, current_pcb);
            return;
        }

        tmp = tmp->next;
    }

    /* can't find other runnable process, try current process */
    if (current_pcb->status == READY) {
        current_pcb->status = RUNNING;
        return;        
    }

    /* can't find any runnable process, switch to idle process */
    pcb_list->status = RUNNING;
    current_pcb = pcb_list;
    ContextSwitch(&switchFunction, &old_pcb->context, old_pcb, current_pcb);
}

#endif /* _pcb_c */