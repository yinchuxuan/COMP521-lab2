#ifndef _trap_kernel_h
#define _trap_kernel_h

#include <comp421/hardware.h>
#include <comp421/yalnix.h>
#include <stdio.h>

void trap_kernel_handler(ExceptionInfo*);

int yalnix_exit_handler(int);

#endif /* _trap_kernel_h */