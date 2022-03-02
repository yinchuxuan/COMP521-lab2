#ifndef _pmem_c
#define _pmem_c

#include "pmem.h"
#include "common.h"
#include "kbrk.h"

struct physical_page_frame* free_page_list;

struct physical_page_frame* page_frame_list;

void init_physical_pages(int pmem_size) {
    int page_num = pmem_size >> PAGESHIFT;
    free_page_list = NULL;
    int i;

    page_frame_list = malloc(page_num * sizeof(struct physical_page_frame));

    for (i = 0; i < page_num; i++) {        // assign pages to free page list
        // Pages in [KERNEL_STACK_BASE, kernel_brk] are already in used
        if (i >= (PAGE_ROUND_DOWN(KERNEL_STACK_BASE) >> PAGESHIFT) && i <= (PAGE_ROUND_DOWN((unsigned int)kernel_brk) >> PAGESHIFT)) {
            page_frame_list[i].next = NULL;
            page_frame_list[i].page_reference = 1;
        } else {
            page_frame_list[i].next = free_page_list;
            page_frame_list[i].page_reference = 0;
            free_page_list = &page_frame_list[i];
        }
    }
}

struct physical_page_frame* allocate_page() {
    if (!free_page_list) {  // run out of physical memory
        printf("Run out of physical memory. Cannot allocate new page!\n");
        exit(1);
    }
    
    struct physical_page_frame* result = free_page_list;
    free_page_list = free_page_list->next;
    result->next = NULL;
    return result;
}

void free_page(struct physical_page_frame* page_frame) {
    if (page_frame->page_reference != 0) {
        printf("Can not free page. This page is still in use!\n");
        return;
    }
    if (page_frame->next) {
        printf("Double free!\n");
        return;
    }

    page_frame->next = free_page_list;
    free_page_list = page_frame;
}

void* page_frame_to_physical_address(struct physical_page_frame* page_frame) {
    return PMEM_BASE + ((unsigned int)(page_frame - page_frame_list) << PAGESHIFT);
}

struct physical_page_frame* physical_address_to_page_frame(void* physical_address) {
    return &page_frame_list[((unsigned int)physical_address - PMEM_BASE) >> PAGESHIFT];
}

#endif /* _pmem_c */