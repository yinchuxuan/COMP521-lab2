#ifndef _boot_h
#define _boot_h

#include <comp421/hardware.h>

struct pte* region1_page_table;

struct pte* initial_region0_page_table;

void setup_region0_page_table(struct pte*);

#endif /* _boot_h */