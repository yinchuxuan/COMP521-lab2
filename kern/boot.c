#ifndef _boot_c
#define _boot_c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "boot.h"
#include "common.h"
#include "kbrk.h"
#include "pmem.h"
#include "pcb.h"
#include "load.h"
#include "trap_kernel.h"
#include "trap_clock.h"
#include "trap_memory.h"
#include "trap_illegal.h"
#include "trap_math.h"

void trap_tty_receive_handler() {
    printf("trap tty receive handler is called!\n");
}

void trap_tty_transmit_handler() {
    printf("trap tty transmit handler is called!\n");
}

void idle_process() {
    printf("this is idle process!\n");
    while (1);
}

void setup_region0_page_table(struct pte* region0_page_table) {
    unsigned int address;
    int vpn;
    struct physical_page_frame* page_frame;

    memset(region0_page_table, 0, PAGE_TABLE_SIZE);

    for (address = VMEM_BASE; address < VMEM_0_LIMIT; address += PAGESIZE) {
        vpn = VPN(address);

        if (address >= DOWN_TO_PAGE(KERNEL_STACK_BASE) && address < DOWN_TO_PAGE(KERNEL_STACK_LIMIT)) {
            page_frame = allocate_page();
            if (!page_frame) {      // allocating physical memory failed
                exit(1); 
            }
            page_frame->page_reference++;

            unsigned int physical_address = page_frame_to_physical_address(page_frame);

            region0_page_table[vpn].pfn = PFN(physical_address);
            region0_page_table[vpn].valid = VALID_PAGE;
            region0_page_table[vpn].uprot = 0;          // Set user no access to the kernel stack
            region0_page_table[vpn].kprot = PAGE_READ | PAGE_WRITE;     // Set kernel read and write access to the kernel stack
        } else {
            /* This PTE is invalid */
            region0_page_table[vpn].pfn = 0;
            region0_page_table[vpn].valid = INVALID_PAGE;
            region0_page_table[vpn].uprot = 0;
            region0_page_table[vpn].kprot = 0;               
        } 
    }
}

void (*interrupt_vector_table[TRAP_VECTOR_SIZE])();

void init_page_table() {
    unsigned int address;
    int vpn;

    memset(initial_region0_page_table, 0, PAGE_TABLE_SIZE);
    memset(region1_page_table, 0, PAGE_TABLE_SIZE);

    for (address = VMEM_BASE; address < VMEM_LIMIT; address += PAGESIZE) {
        if (address < VMEM_0_LIMIT) {   // In region0
            vpn = VPN(address);

            if (address >= DOWN_TO_PAGE(KERNEL_STACK_BASE) && address < DOWN_TO_PAGE(KERNEL_STACK_LIMIT)) {
                /* Identity mapping for [KERNEL_STACK_BASE, KERNEL_STACK_LIMIT] in region0 */
                initial_region0_page_table[vpn].pfn = PFN(address);
                initial_region0_page_table[vpn].valid = VALID_PAGE;
                initial_region0_page_table[vpn].uprot = 0;          // Set user no access to the kernel stack
                initial_region0_page_table[vpn].kprot = PAGE_READ | PAGE_WRITE;     // Set kernel read and write access to the kernel stack
            } else {
                /* This PTE is invalid */
                initial_region0_page_table[vpn].pfn = 0;
                initial_region0_page_table[vpn].valid = INVALID_PAGE;
                initial_region0_page_table[vpn].uprot = 0;
                initial_region0_page_table[vpn].kprot = 0;               
            } 
        } else {
            vpn = VPN(address - VMEM_1_BASE);

            if (address >= DOWN_TO_PAGE(VMEM_1_BASE) && address <= DOWN_TO_PAGE((unsigned int)kernel_brk)) {
                /* Identity mapping for [VMEM_1_BASE, kernel_brk] in region1 */
                region1_page_table[vpn].pfn = PFN(address);
                region1_page_table[vpn].valid = VALID_PAGE;
                region1_page_table[vpn].uprot = 0;          // Set user no access to the kernel

                if (address < &_etext) {
                    region1_page_table[vpn].kprot = PAGE_READ | PAGE_EXECUTE;     // Set kernel read and execute access to the kernel text 
                } else {
                    region1_page_table[vpn].kprot = PAGE_READ | PAGE_WRITE;     // Set kernel read and write access to the kernel data/bss/heap 
                }
            } else {
                /* This PTE is invalid */
                region1_page_table[vpn].pfn = 0;
                region1_page_table[vpn].valid = INVALID_PAGE;
                region1_page_table[vpn].uprot = 0;
                region1_page_table[vpn].kprot = 0;
            }
        }
    }

}

void KernelStart(ExceptionInfo * info, unsigned int pmem_size, void * origin_brk, char ** args) {
    /* Initialize kernel break */
    kernel_brk = origin_brk;

    /* Initialize interrupt vector table */
    int i;
    for (i = 0; i < TRAP_VECTOR_SIZE; i++) {
        switch (i) {
            case TRAP_KERNEL:
                interrupt_vector_table[i] = &trap_kernel_handler;
                break;
            case TRAP_CLOCK:
                interrupt_vector_table[i] = &trap_clock_handler;
                break;
            case TRAP_ILLEGAL:
                interrupt_vector_table[i] = &trap_illegal_handler;
                break;
            case TRAP_MEMORY:
                interrupt_vector_table[i] = &trap_memory_handler;
                break;
            case TRAP_MATH:
                interrupt_vector_table[i] = &trap_math_handler;
                break;
            case TRAP_TTY_RECEIVE:
                interrupt_vector_table[i] = &trap_tty_receive_handler;
                break;
            case TRAP_TTY_TRANSMIT:
                interrupt_vector_table[i] = &trap_tty_transmit_handler;
                break;
            default:
                interrupt_vector_table[i] = NULL;
                break;
        }
    }

    WriteRegister(REG_VECTOR_BASE, (RCS421RegVal)interrupt_vector_table);

    /* Allocate memory for page table before physical page initialization */
    region1_page_table = (struct pte*)malloc(PAGE_TABLE_SIZE);
    initial_region0_page_table = (struct pte*)malloc(PAGE_TABLE_SIZE);

    /* Initialize physical pages */
    init_physical_pages(pmem_size);

    /* Initialize page table */
    init_page_table();
    WriteRegister(REG_PTR0, (RCS421RegVal)initial_region0_page_table);
    WriteRegister(REG_PTR1, (RCS421RegVal)region1_page_table);
    
    /* Enable virtual memory */
    WriteRegister(REG_VM_ENABLE, 1);
    is_vm_on = 1;

    /* Initialize some pcb list */
    delayed_pcb_list = (struct delayed_pcb*)malloc(sizeof(struct delayed_pcb));     // make a pesudo head
    delayed_pcb_list->next = NULL;

    /* Create first idle process */
    pcb_list = (struct pcb*)malloc(sizeof(struct pcb));
    pcb_list->next = NULL;
    pcb_list->pid = 0;
    pcb_list->clock_ticks = 0;
    pcb_list->status = READY;
    pcb_list->user_brk = MEM_INVALID_SIZE;
    pcb_list->user_stack_base = USER_STACK_LIMIT; 
    pcb_list->region0_page_table = (struct pte*)malloc(PAGE_TABLE_SIZE);
    setup_region0_page_table(pcb_list->region0_page_table);

    /* Create init process */
    struct pcb* new_pcb = (struct pcb*)malloc(sizeof(struct pcb));
    new_pcb->next = pcb_list->next;
    pcb_list->next = new_pcb;
    new_pcb->pid = 1;
    new_pcb->clock_ticks = 0;
    new_pcb->status = READY;
    new_pcb->user_brk = MEM_INVALID_SIZE;
    new_pcb->user_stack_base = USER_STACK_LIMIT;
    new_pcb->region0_page_table = initial_region0_page_table; 

    current_pcb = pcb_list->next;

    /* save context for idle process */
    ContextSwitch(switchFunction, &pcb_list->context, pcb_list, pcb_list);
    if (current_pcb->pid == 0) {    // run idle process as a user process
        info->pc = &idle_process;
        info->sp = KERNEL_STACK_LIMIT;
        info->psr = PSR_MODE;
        return;
    }

    /* load init process */
    int ret = LoadProgram("init", args);
    if (ret == -1) {
        printf("load program with still runnable error!\n");
    }
    if (ret == -2) {
        printf("load program with no longer runnable error!\n");
        exit(1);
    }

    info->pc = current_pcb->pc;
    info->sp = current_pcb->sp;
    for (i = 0; i < NUM_REGS; i++) {
        info->regs[i] = current_pcb->regs[i]; 
    }
    info->psr = current_pcb->psr;
    current_pcb->status = RUNNING;
}

#endif /* _boot_c */