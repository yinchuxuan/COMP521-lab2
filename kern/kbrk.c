#ifndef _kbrk_c
#define _kbrk_c

#include "kbrk.h"
#include "boot.h"
#include "pmem.h"
#include "common.h"

int SetKernelBrk(void* new_kernel_brk) {
    if (!is_vm_on) {
        kernel_brk = new_kernel_brk;
    } else {
        unsigned int address;
        int vpn;
        struct physical_page_frame* physical_page;

        printf("set kernel brk is called after paging!\n");

        if (new_kernel_brk > VMEM_1_LIMIT) {
            printf("Invalid address of kernel break: %x", (unsigned int)new_kernel_brk);
            return -1;
        }

        for (address = DOWN_TO_PAGE(kernel_brk); address <= UP_TO_PAGE(new_kernel_brk); address += PAGESIZE) {
            vpn = VPN(address - VMEM_1_BASE);

            printf("allocate memory for page %d\n", vpn);

            if (!region1_page_table[vpn].valid) {   // This page is not allocated yet
                if (!(physical_page = allocate_page())) {    // Run out of physical memory
                    return -1;
                }

                physical_page->page_reference++;
                region1_page_table[vpn].valid = VALID_PAGE;
                region1_page_table[vpn].pfn = PFN(page_frame_to_physical_address(physical_page));
                region1_page_table[vpn].kprot = PAGE_READ | PAGE_WRITE;
                region1_page_table[vpn].uprot = 0;
            }
        }
    }

    return 0;
}

#endif /* _kbrk_c */