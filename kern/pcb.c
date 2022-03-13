#ifndef _pcb_c
#define _pcb_c

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

    /* just for saving context */
    if (old_pcb_p->pid == new_pcb_p->pid) {
        return &old_pcb_p->context;
    }

    /* switch region0 table page */
    WriteRegister(REG_PTR0, (RCS421RegVal)new_pcb_p->region0_page_table);
    WriteRegister(REG_TLB_FLUSH, TLB_FLUSH_ALL);

    return &new_pcb_p->context;
}

struct pcb* create_process() {
    struct pcb* new_pcb = (struct pcb*)malloc(sizeof(struct pcb));
    pcb_list_tail->next = new_pcb;
    pcb_list_tail = new_pcb;
    new_pcb->pid = next_pid++;
    new_pcb->status = NEW;
    new_pcb->region0_page_table = (struct pte*)malloc(PAGE_TABLE_SIZE);
    if (!new_pcb->region0_page_table) {
        // deal with allocation failure
    }

    setup_region0_page_table(new_pcb->region0_page_table);
    return new_pcb;
}

#endif /* _pcb_c */