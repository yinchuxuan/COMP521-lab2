#ifndef _pmem_h 
#define _pmem_h 

#include <comp421/hardware.h>
#include <stdio.h>

/*
 * Physical page frame structure
 * This frame structure represents a real physical page in memory. This
 * frame structure supports linked list structure for free page list. page
 * reference stands for the current reference of this page.
 */
struct physical_page_frame {
    struct physical_page_frame* next;
    int page_reference;
};

/*
 * Init physical page frames and free page list.
 */
void init_physical_pages(int);

/*
 * Allocate a new physical page from free page list.
 * If there are no more free page, exit
 */
struct physical_page_frame* allocate_page();

/*
 * Free a physical page. Return it to free page list.
 */
void free_page(struct physical_page_frame*);

/*
 * Transfer page frame to real physical address.
 */
unsigned int page_frame_to_physical_address(struct physical_page_frame*);

/*
 * Transfer physical address to page frame.
 */
struct physical_page_frame* physical_address_to_page_frame(void*);

#endif /* _pmem_h */