#ifndef _pcb_h
#define _pcb_h

#include "hardware.h"

/*
 * Status of process
 */
enum Status {
    NEW = 0,
    RUNNING,
    WAITING,
    READY,
    TERMINATED 
};

/*
 * Process control block
 * This structure represents a process in the operating system, which supports
 * a linked list structure of process block.
 */
struct pcb {
    int pid;        /* process id */
    enum Status status;         /* process status */
    SavedContext context;          /* Machine context saved at time of switching out */
    struct pte* region0_page_table;     /* page table of region0 in process */
    unsigned long psr;	    /* processor status register */
    void *pc;		/* PC at time of interrupt */
    void *sp;		/* SP at time of interrupt */
    unsigned long regs[NUM_REGS]; /* general registers at time of interrupt */
    struct pcb* next;       /* next process control block */
};

struct pcb* pcb_list;

struct pcb* current_pcb;

struct pcb* pcb_list_tail;

SavedContext* switchFunction(SavedContext*, void*, void*);

struct pcb* create_process();

#endif /* _pcb_h */