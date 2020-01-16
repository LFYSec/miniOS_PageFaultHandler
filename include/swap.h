#ifndef SWAP_H
#define SWAP_H

#include "type.h"
#include "proc.h"
#include "const.h"
#include "proto.h"
#include "memman.h"

/* TODO: pass a pointer to a page table entry, 
 * don't need a page dir address. like this:
 * void swapout(struct Page *pdesc, u32 pte_ptr);
 * params:
 *     pdesc: pointer to a page descriptor;
 *     pte_ptr: pointer to a page table *entry*
 *         which references the page pointed by `pdesc`;
 * */

void swapout(struct Page *pdesc, u32 pd);

void swapin(struct Page *pdesc, u32 pd);

/*
 * return: swap index */
u32 move_page_to_swap(u32 vaddr);

void move_page_from_swap(u32 swap_idx, u32 vaddr);

void rw_sector_swap(int io_type, u64 pos, void* buf);

#endif /* end of include guard: SWAP_H */
