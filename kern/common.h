#ifndef _common_h
#define _common_h

#include "hardware.h"

#define VPN(va) (va >> PAGESHIFT)

#define PFN(pa) (pa >> PAGESHIFT)

#define PAGE_ROUND_DOWN(va) ((va >> PAGESHIFT) << PAGESHIFT)

#define PAGE_ROUND_UP(va) (va % PAGESHIFT ? ((va >> PAGESHIFT) + 1) << PAGESHIFT : va)

#define VALID_PAGE 1

#define INVALID_PAGE 0

#define PAGE_READ 0x001

#define PAGE_WRITE 0x002

#define PAGE_EXECUTE 0x004

#endif /* _common_h */