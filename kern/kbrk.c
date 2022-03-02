#ifndef _kbrk_c
#define _kbrk_c

#include "kbrk.h"

int SetKernelBrk(void* new_kernel_brk) {
    if (!is_vm_on) {
        kernel_brk = new_kernel_brk;
    }

    return 0;
}

#endif /* _kbrk_c */