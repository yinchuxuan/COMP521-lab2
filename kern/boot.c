#ifndef _boot_c
#define _boot_c

#include <stdio.h>
#include "boot.h"
#include "common.h"
#include "kbrk.h"
#include "pmem.h"

void trap_kernel_handler() {
   printf("trap kernel handler is called!\n"); 
}

void trap_clock_handler() {
    printf("trap clock handler is called!\n");
}

void trap_illegal_handler() {
    printf("trap illegal handler is called!\n");
}

void trap_memory_handler() {
    printf("trap memory handler is called!\n");
}

void trap_math_handler() {
    printf("trap math handler is called!\n");
}

void trap_tty_receive_handler() {
    printf("trap tty receive handler is called!\n");
}

void trap_tty_transmit_handler() {
    printf("trap tty transmit handler is called!\n");
}

void (*interrupt_vector_table[TRAP_VECTOR_SIZE])();

void init_page_table() {
    unsigned int address;
    int vpn;

    for (address = VMEM_BASE; address < VMEM_LIMIT; address += PAGESIZE) {
        if (address < VMEM_0_LIMIT) {   // In region0
            vpn = VPN(address);

            if (address >= PAGE_ROUND_DOWN(KERNEL_STACK_BASE) && address < PAGE_ROUND_DOWN(KERNEL_STACK_LIMIT)) {
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

            if (address >= PAGE_ROUND_DOWN(VMEM_1_BASE) && address <= PAGE_ROUND_DOWN((unsigned int)kernel_brk)) {
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
    interrupt_vector_table[TRAP_KERNEL] = &trap_kernel_handler;
    interrupt_vector_table[TRAP_CLOCK] = &trap_clock_handler;
    interrupt_vector_table[TRAP_ILLEGAL] = &trap_illegal_handler;
    interrupt_vector_table[TRAP_MEMORY] = &trap_memory_handler;
    interrupt_vector_table[TRAP_MATH] = &trap_math_handler;
    interrupt_vector_table[TRAP_TTY_RECEIVE] = &trap_tty_receive_handler;
    interrupt_vector_table[TRAP_TTY_TRANSMIT] = &trap_tty_transmit_handler;

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

}

#endif /* _boot_c */