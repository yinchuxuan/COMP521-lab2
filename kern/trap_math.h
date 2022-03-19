#ifndef _trap_math_h
#define _trap_math_h

#include <comp421/hardware.h>
#include <comp421/yalnix.h>

#define MAX_CLOCK_TICKS 2

void trap_math_handler(ExceptionInfo*);

#endif /* _trap_math_h */