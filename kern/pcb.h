#ifndef _pcb_h
#define _pcb_h

#include <comp421/hardware.h>

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
    int clock_ticks;        /* clock ticks that current process already run across */
    unsigned int user_brk;  /* user break in current process */
    unsigned int user_stack_base;   /* user stack base in current process */
};

struct delayed_pcb {
    struct pcb* process;
    int remaining_clock_ticks;
    struct delayed_pcb* next;
};

struct pcb* pcb_list;

struct pcb* current_pcb;

struct delayed_pcb* delayed_pcb_list; 

SavedContext* switchFunction(SavedContext*, void*, void*);

struct pcb* create_process();

void terminate_process(struct pcb*);

void round_robin_process_schedule();

#endif /* _pcb_h */