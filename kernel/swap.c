#include "swap.h"
#include "klog.h"
#include "type.h"
#include "fs_const.h"

#define PAGE_SIZE 4096
#define SWAP_PAGES 1024
#define SWAP_BITMAP_BYTES SWAP_PAGES / 8

inline static u32 pd_index(u32 vaddr)
{
    return vaddr >> 22;
}

inline static u32 pt_index(u32 vaddr)
{
    return (vaddr & 0x003FFFFF) >> 12;
}

inline static u32 get_pt_paddr(u32 pd, u32 vaddr)
{
    return (*((u32 *)K_PHY2LIN(pd) + pd_index(vaddr))) \
        & 0xFFFFF000;
}

inline static u32 get_page_paddr(u32 pt, u32 vaddr)
{
    return (*(u32 *)(K_PHY2LIN(pt) + pt_index(vaddr))) \
        & 0xFFFFF000;
}

inline static void setbit(void *addr, u32 pos, int v)
{
    u32 u = ((u32 *)addr)[pos / 32];
    if (v == 0) {
        u &= 0xFFFFFFFE << (pos % 32);
    } else {
        u |= 1 << (pos % 32);
    }
    ((u32 *)addr)[pos / 32] = u;
}

struct swapfile {
    u32 version;
    u32 pages;
    u32 *bitmap;
    u32 next_avail;
};

struct swapfile swap_master;

void swapinit()
{
    swap_master.pages = SWAP_PAGES;
    swap_master.bitmap = K_PHY2LIN(do_kmalloc(SWAP_BITMAP_BYTES));
    swap_master.version = 0;
    swap_master.next_avail = 0;
    memset(swap_master.bitmap, '\0', SWAP_BITMAP_BYTES);
}

u32 swap_next_avail(struct swapfile *swap)
{
    u32 idx;
    if (swap->next_avail >= swap->pages) {
        swap->next_avail = 0;
    }

    idx = swap->next_avail;
    u32 bitmap_len_u32 = swap->pages / 32;
    for (u32 i = 0; i < bitmap_len_u32; i++) {
        for (u32 bits = swap->bitmap[i], j = 0; j < 32; j++) {
            /*printf("checking %d\n", bits);*/
            if (!(bits & 0x00000001)) {
                idx = i * 32 + j;
                setbit(swap->bitmap, idx, 1);
                goto ret;
            }
            bits >>= 1;
        }
    }
ret:
    return idx;
}

// TODO: We should pass a physical address, because we can't access
// memory of other processes via linear address so that we can't
// swapout/in in a kernel task. To fix this issue, we could map 
// a temp page for kernel to access these phusical address.
u32 move_page_to_swap(u32 vaddr)
{
    u32 idx;

    kassert(vaddr == (vaddr & 0xFFFFF000));
    idx = swap_next_avail(&swap_master);

    /* then write page in page frame to swap's idx block */
    /* TODO */

    rw_sector_swap(
        DEV_WRITE,
        idx * PAGE_SIZE,
        (void*)vaddr
    );

    char buf[128];
    char buf1[10];
    itoa(buf1, vaddr);
    sprintf(buf, "move page %s to swap %d", buf1, idx);
    kdebug(buf);
    return idx;
}

void move_page_from_swap(u32 swap_idx, u32 vaddr)
{
    kassert(vaddr == (vaddr & 0xFFFFF000));
    setbit(swap_master.bitmap, swap_idx, 0);

    /* then read page in swap and write to vaddr in memory */
    /* TODO */
    rw_sector_swap(
        DEV_READ,
        swap_idx * PAGE_SIZE,
        (void*)vaddr
    );

    char buf[128];
    sprintf(buf, "move page from swap %d to %d", swap_idx, vaddr);
    kdebug(buf);
}

void swapout(struct Page *pdesc, u32 pd)
{
    u32 page_vaddr, page_paddr, // address of page
        pt;  // physics address of page table
    kdebug("swapout");

    page_vaddr = pdesc->v_addr;

    kassert(pdesc->flag == PG_ALLOCATED);
    kassert(page_vaddr == (page_vaddr & 0xFFFFF000));
    // should be a user address
    kassert(page_vaddr < KernelLinBase);
    // page table should existed.
    kassert(pte_exist(pd, page_vaddr));

    pt = get_pt_paddr(pd, page_vaddr);
    page_paddr = get_page_paddr(pt, page_vaddr);

    // update page descriptor
    // TODO: fail check
    pdesc->swap_index = move_page_to_swap(page_vaddr);
    pdesc->flag = PG_SWAPOUT;

    // update page table entry
    write_page_pte(pt, page_vaddr, 0,
                   PG_USU | PG_RWW);
    /*refresh_page_cache();*/

    // free from memman
    do_free_4k(page_paddr);
}

void swapin(struct Page *pdesc, u32 pd)
{
    u32 page_vaddr, page_paddr, // address of page
        pt;  // physics address of page table
    kdebug("swapin");

    page_vaddr = pdesc->v_addr;

    kassert(pdesc->flag == PG_SWAPOUT);
    kassert(page_vaddr == (page_vaddr & 0xFFFFF000));
    // should be a user address
    kassert(page_vaddr < KernelLinBase);
    // page table should existed.
    kassert(pte_exist(pd, page_vaddr));

    pt = get_pt_paddr(pd, page_vaddr);
    page_paddr = get_page_paddr(pt, page_vaddr);

    page_paddr = do_malloc_4k();
    // update page table entry
    write_page_pte(pt, page_vaddr, page_paddr,
                   PG_P | PG_USU | PG_RWW);
    refresh_page_cache();

    // update page descriptor
    // TODO: fail check
    move_page_from_swap(pdesc->swap_index, page_vaddr);
    pdesc->flag = PG_ALLOCATED;
}

void rw_sector_swap(int io_type, u64 pos, void* buf)
{
    MESSAGE driver_msg;
    
    driver_msg.type		= io_type;
    driver_msg.DEVICE	= MINOR(SWAP_DEV);
    driver_msg.POSITION	= pos;
    driver_msg.CNT		= PAGE_SIZE;
    driver_msg.PROC_NR	= -1;
    driver_msg.BUF		= buf;
    
    hd_rdwt(&driver_msg);
}
