#ifndef _trap_illegal_h
#define _trap_illegal_h

#include <comp421/hardware.h>
#include <comp421/yalnix.h>

#define MAX_CLOCK_TICKS 2

void trap_illegal_handler(ExceptionInfo*);

#endif /* _trap_illegal_h */