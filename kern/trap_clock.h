#ifndef _trap_clock_h
#define _trap_clock_h

#include <comp421/hardware.h>
#include <comp421/yalnix.h>

#define MAX_CLOCK_TICKS 2

void trap_clock_handler(ExceptionInfo*);

#endif /* _trap_clock_h */