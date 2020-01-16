
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                            global.c
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
                                                    Forrest Yu, 2005
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* 
 * To make things more direct. In the headers below, if a variable declaration has a EXTERN prefix,
 * the variable will be defined here.
 * added by xw, 18/6/17
 */
// #define GLOBAL_VARIABLES_HERE
#include "const.h"
#undef	EXTERN	//EXTERN could have been defined as extern in const.h
#define	EXTERN	//redefine EXTERN as nothing
#include "type.h"
#include "protect.h"
#include "proc.h"
#include "global.h"
#include "proto.h"
#include "fs_const.h"
#include "hd.h"
#include "fs.h"
#include "fat32.h"	//added by mingxuan 2019-5-17
#include "vfs.h"	//added by mingxuan 2019-5-17
#include "klog.h"
#include "swap.h"

static void swaptest()
{
    
	while(kernel_initial){}

	char buf[520] = "111111111111111111\0";
	hd_open(MINOR(SWAP_DEV));

	rw_sector_swap(
        DEV_WRITE,
        0,
        buf
    );

	/*
    link_foreach(&p_proc_current->task.pages, p) {
        struct Page *page = link_entry(p, struct Page, node);
        if (page->flag == PG_ALLOCATED) {
            swapout(page, p_proc_current->task.cr3);
            disp_str("swap");
    		break;
        }
    }
	char buf[520] = "1\0";

	kdebug("buf_raw:");
    kdebug(buf);

	rw_sector_swap(
        DEV_READ,
        0,
        buf
    );

	kdebug("buf_final:");
    kdebug(buf);
	
	
	char wr_test[512] = "12312123123123123123123132";
	char rd_test[512];

	rw_sector_swap(
		DEV_WRITE,
		10 * SECTOR_SIZE,
		wr_test
	);

	rw_sector_swap(
		DEV_READ,
		10 * SECTOR_SIZE,
		rd_test
	);

	kdebug(rd_test);

	*/

	/*Page page = link_entry(proc_table[4].task.pages.next, struct Page, node);*/
    /*while (proc_table[4].task.p_name[0] != 'o');*/
    /*while(strcmp(proc_table[4].task.p_name, "orange/shell_0.b"));*/
	/*disp_str("11111111");*/

    /*swapout(page, proc_table[4].task.cr3);*/
	//swapout(page, proc_table[4].task.cr3);

	//swapin(page->swap_index, page);
	
    while (1);
}

/* save the execution environment of each cpu, which doesn't belong to any process.
 * added by xw, 18/6/1
 */
PUBLIC	PROCESS			cpu_table[NR_CPUS];

PUBLIC	PROCESS			proc_table[NR_PCBS];										//edit by visual 2016.4.5	

//PUBLIC	char			task_stack[STACK_SIZE_TOTAL]; //delete  by viusal 2016.4.28

PUBLIC	TASK	task_table[NR_TASKS] = {//{TestA, STACK_SIZE_TASK, "TestA"},	//edit by visual 2016.4.5	//deleted by mingxuan 2019-5-19
										//{TestB, STACK_SIZE_TASK, "TestB"},	//deleted by mingxuan 2019-5-19
										//{TestC, STACK_SIZE_TASK, "TestC"},	//deleted by mingxuan 2019-5-19
                                        {swaptest, STACK_SIZE_TASK, "swaptest"},
										{hd_service, STACK_SIZE_TASK, "hd_service"},
                                        {task_tty, STACK_SIZE_TASK, "task_tty"},
};	//added by xw, 18/8/27


PUBLIC	irq_handler		irq_table[NR_IRQ];

PUBLIC	system_call		sys_call_table[NR_SYS_CALL] = {	sys_get_ticks, 									//1st
														sys_get_pid,		//add by visual 2016.4.6
														sys_kmalloc,		//add by visual 2016.4.6 
														sys_kmalloc_4k,		//add by visual 2016.4.7
														sys_malloc,			//add by visual 2016.4.7	//5th
														sys_malloc_4k,		//add by visual 2016.4.7 
														sys_free,			//add by visual 2016.4.7 
														sys_free_4k,		//add by visual 2016.4.7 
														sys_fork,			//add by visual 2016.4.8 
														sys_pthread,		//add by visual 2016.4.11	//10th
														sys_udisp_int,		//add by visual 2016.5.16 
														sys_udisp_str,		//add by visual 2016.5.16
														sys_exec,			//add by visual 2016.5.16
														sys_yield,			//added by xw
													    sys_sleep,			//added by xw				//15th
													    sys_print_E,		//added by xw
													    sys_print_F,		//added by xw
														sys_open,			//added by xw, 18/6/18
													    sys_close,			//added by xw, 18/6/18
													    sys_read,			//added by xw, 18/6/18		//20th
													    sys_write,			//added by xw, 18/6/18
													    sys_lseek,			//added by xw, 18/6/18
														sys_unlink,			//added by xw, 18/6/19		//23th

														sys_create,			//added by mingxuan 2019-5-17
														sys_delete,			//added by mingxuan 2019-5-17
														sys_opendir,		//added by mingxuan 2019-5-17
														sys_createdir,		//added by mingxuan 2019-5-17
														sys_deletedir		//added by mingxuan 2019-5-17
														};

PUBLIC TTY tty_table[NR_CONSOLES];			//added by mingxuan 2019-5-19
PUBLIC CONSOLE console_table[NR_CONSOLES];	//added by mingxuan 2019-5-19
