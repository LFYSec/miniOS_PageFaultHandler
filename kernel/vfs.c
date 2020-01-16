/**********************************************************
*	vfs.c       //added by mingxuan 2019-5-17
***********************************************************/

#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "proto.h"
#include "fs_const.h"
#include "hd.h"
#include "fs.h"
#include "fs_misc.h"
#include "vfs.h"
#include "fat32.h"

PRIVATE struct file_op f_op_table[NR_fs];//文件系统操作表
PRIVATE struct device  device_table[NR_DEV];
//PRIVATE struct file_desc new_file_desc[10];
PUBLIC struct file_desc f_desc_table[NR_FILE_DESC];
PRIVATE int strcmp(const char *s1, const char *s2);
PRIVATE void init_dev_table();
PUBLIC void init_fileop_table();
PRIVATE int get_index(char path[]);

PUBLIC void init_vfs(){
    init_fileop_table();
    init_dev_table();
}

PRIVATE void init_dev_table(){
    device_table[0].dev_name="dev_tty0";
    device_table[0].op = &f_op_table[0];
    device_table[1].dev_name="dev_tty1";
    device_table[1].op =&f_op_table[0];
    device_table[2].dev_name="dev_tty2";
    device_table[2].op=&f_op_table[0];
    device_table[3].dev_name="fat0";
    device_table[3].op=&f_op_table[2];
    device_table[4].dev_name="orange";
    device_table[4].op=&f_op_table[1];
}

PUBLIC void init_fileop_table(){
    // table[0] for tty
    // added by mingxuan 2019-5-19
    f_op_table[0].open = real_open;
    f_op_table[0].close = real_close;
    f_op_table[0].write = real_write;
    f_op_table[0].lseek = real_lseek;
    f_op_table[0].unlink = real_unlink;
    f_op_table[0].read = real_read;

    f_op_table[1].open = real_open;
    f_op_table[1].close = real_close;
    f_op_table[1].write = real_write;
    f_op_table[1].lseek = real_lseek;
    f_op_table[1].unlink = real_unlink;
    f_op_table[1].read = real_read;

    f_op_table[2].create = CreateFile;
    f_op_table[2].delete = DeleteFile;
    f_op_table[2].open = OpenFile;
    f_op_table[2].close = CloseFile;
    f_op_table[2].write = WriteFile;
    f_op_table[2].read = ReadFile;
    f_op_table[2].opendir = OpenDir;
    f_op_table[2].createdir = CreateDir;
    f_op_table[2].deletedir = DeleteDir;
    //f_op_table[2].lseek = real_lseek;
}

PRIVATE int get_index(char path[]){

    int pathlen = strlen(path);
    char dev_name[DEV_NAME_LEN];
    int len = (pathlen < DEV_NAME_LEN) ? pathlen : DEV_NAME_LEN;
    
    int i,a=0;
    for(i=0;i<len;i++){
        if( path[i] == '/'){
            a=i;
            a++;
            break;
        }
        else {
            dev_name[i] = path[i];
        }
    }
    dev_name[i] = '\0';
    for(i=0;i<pathlen-a;i++)
        path[i] = path[i+a];
    path[pathlen-a] = '\0';

    for(i=0;i<NR_DEV;i++)
    {
        if(!strcmp(dev_name,device_table[i].dev_name))
            return i;
    }

    return -1;
}


/*======================================================================*
                              sys_* 系列函数
 *======================================================================*/

PUBLIC int sys_open(void *uesp)
{
    return do_vopen(get_arg(uesp, 1), get_arg(uesp, 2));
}

PUBLIC int sys_close(void *uesp)
{
    return do_vclose(get_arg(uesp, 1));
}

PUBLIC int sys_read(void *uesp)
{
    return do_vread(get_arg(uesp, 1), get_arg(uesp, 2), get_arg(uesp, 3));
}

PUBLIC int sys_write(void *uesp)
{
    return do_vwrite(get_arg(uesp, 1), get_arg(uesp, 2), get_arg(uesp, 3));
}

PUBLIC int sys_lseek(void *uesp)
{
    return do_vlseek(get_arg(uesp, 1), get_arg(uesp, 2), get_arg(uesp, 3));
}

PUBLIC int sys_unlink(void *uesp) {
    return do_vunlink(get_arg(uesp, 1));
}

PUBLIC int sys_create(void *uesp) {
    return do_vcreate(get_arg(uesp, 1));
}

PUBLIC int sys_delete(void *uesp) {
    return do_vdelete(get_arg(uesp, 1));
}

PUBLIC int sys_opendir(void *uesp) {
    return do_vopendir(get_arg(uesp, 1));
}

PUBLIC int sys_createdir(void *uesp) {
    return do_vcreatedir(get_arg(uesp, 1));
}

PUBLIC int sys_deletedir(void *uesp) {
    return do_vdeletedir(get_arg(uesp, 1));
}


/*======================================================================*
                              do_v* 系列函数
 *======================================================================*/

PUBLIC int do_vopen(const char *path, int flags) {

    int pathlen = strlen(path);
    char pathname[MAX_PATH];
    
    strcpy(pathname,path);
    pathname[pathlen] = 0;

    int index,i;
    int fd = -1;
    index = get_index(pathname);
    if(index == -1){
        disp_str("pathname error!\n");
        disp_str(path);
        return -1;
    }

    //added by mingxuan 2019-5-18
    //deleted by mingxuan 2019-5-19
    //if(index == 3) //index=3表示是fat32 
    //    do_vcreate(path); //fat32必须先调用create，之后才能调用open

    fd = device_table[index].op->open(pathname, flags);
    //disp_str("fd=:"); //deleted by mingxuan 2019-5-22
    //disp_int(fd);  //deleted by mingxuan 2019-5-22
    if(fd != -1)
    {
        p_proc_current -> task.filp[fd] -> dev_index = index;
        //disp_str("          open file success!\n");   //deleted by mingxuan 2019-5-22
    } else {
        disp_str("          error!\n");
    }
                   
    return fd;    
}


PUBLIC int do_vclose(int fd) {
    int index = p_proc_current->task.filp[fd]->dev_index;

    return device_table[index].op->close(fd);
    // if (state == 1) {
    //     debug("          close file success!");
    // } else {
	// 	DisErrorInfo(state);
    // }
}

PUBLIC int do_vread(int fd, char *buf, int count) {
    //disp_int(fd);
    int index = p_proc_current->task.filp[fd]->dev_index;
    //disp_int(index);
    return device_table[index].op->read(fd, buf, count);
    // if (size >= 0) {
    //     debug("          read file success!");
    // } else {
	// 	DisErrorInfo(size);
    // }
    // return size;
}

PUBLIC int do_vwrite(int fd, const char *buf, int count) {
    //char s[256];  //deleted by mingxuan 2019-5-19
    /*  //deleted by mingxuan 2019-5-23
    char s[FILE_MAX_LEN]; //modified by mingxuan 2019-5-19

    strcpy(s, buf);

    int index = p_proc_current->task.filp[fd]->dev_index;
    return device_table[index].op->write(fd,s,strlen(s));
    */

    //modified by mingxuan 2019-5-23
    char s[512];
    int index = p_proc_current->task.filp[fd]->dev_index;
    char *fsbuf = buf;
    int f_len = count;
    int bytes;
    while(f_len)
    {
        int iobytes = min(512, f_len);
        int i=0;
        for(i=0; i<iobytes; i++)
        {
            s[i] = *fsbuf;
            fsbuf++;
        }
        bytes = device_table[index].op->write(fd,s,iobytes);
        if(bytes != iobytes)
        {
            return bytes;
        }
        f_len -= bytes;
    }
    return count;

    // int fd = get_arg(uesp,1);
    // int index = p_proc_current->task.vfs_filp[fd]->index_of_optable;
    // int d = p_proc_current -> task.vfs_filp[fd] -> fd;
    // int state = f_op_table[index].write(get_arg(uesp,2), get_arg(uesp,3), d);
    // if (state == 1) {
    //     debug("          write file success!");
    // } else {
	// 	DisErrorInfo(state);
    // }
}

PUBLIC int do_vunlink(const char *path) {
    int pathlen = strlen(path);
    char pathname[MAX_PATH];
    
    strcpy(pathname,path);
    pathname[pathlen] = 0;

    int index;
    index = get_index(pathname);
    if(index==-1){
        disp_str("pathname error!\n");
        return -1;
    }
    
    return device_table[index].op->unlink(pathname);
}

PUBLIC int do_vlseek(int fd, int offset, int whence) {
    int index = p_proc_current->task.filp[fd]->dev_index;

    return device_table[index].op->lseek(fd, offset, whence);

}

//PUBLIC int do_vcreate(char *pathname) {
PUBLIC int do_vcreate(char *filepath) { //modified by mingxuan 2019-5-17
    // disp_str("hhh");
    // const char *path = get_arg(uesp,1);

    // int pathlen = strlen(path);
    // char pathname[MAX_PATH];
    
    // strcpy(pathname,path);
    // pathname[pathlen] = '\0';

    // int index;
    // index = get_index(pathname);
    // disp_int(index);
    // if(index==-1){
    //     disp_str("          pathname error!\n");
    //     return -1;
    // }


    // int state = 0;
    
    // //int state = device_table[index].op -> create(pathname);
    // if (state == 1) {
    //     debug("          create file success!");
    // } else {
	// 	DisErrorInfo(state);
    // }
    // return state; 

    //added by mingxuan 2019-5-17  
    int state;
    const char *path = filepath;

    int pathlen = strlen(path);
    char pathname[MAX_PATH];
    
    strcpy(pathname,path);
    pathname[pathlen] = 0;

    int index;
    //index = (int)(pathname[1]-'0');   //deleted by mingxuan 2019-5-17
    //disp_int(index);  //deleted by mingxuan 2019-5-17
    index = get_index(pathname);
    if(index == -1){
        disp_str("pathname error!\n");
        disp_str(path);
        return -1;
    }
    /*
    for(int j=0;j<= pathlen-3;j++)
    {
        pathname[j] = pathname[j+3];
    }
    */
    //state = f_op_table[index].create(pathname);
    state = device_table[index].op->create(pathname);
    if (state == 1) {
        debug("          create file success!");
    } else {
		DisErrorInfo(state);
    }
}

PUBLIC int do_vdelete(char *path) {

    int pathlen = strlen(path);
    char pathname[MAX_PATH];
    
    strcpy(pathname,path);
    pathname[pathlen] = 0;

    int index;
    index = get_index(pathname);
    if(index==-1){
        disp_str("pathname error!\n");
        return -1;
    }
    return device_table[index].op->delete(pathname);
    // state = f_op_table[index].delete(pathname);
    // if (state == 1) {
    //     debug("          delete file success!");
    // } else {
	// 	DisErrorInfo(state);
    // }   
}
PUBLIC int do_vopendir(char *path) {
    int state;

    int pathlen = strlen(path);
    char pathname[MAX_PATH];
    
    strcpy(pathname,path);
    pathname[pathlen] = 0;

    int index;
    index = (int)(pathname[1]-'0');

    for(int j=0;j<= pathlen-3;j++)
    {
        pathname[j] = pathname[j+3];
    }
    state = f_op_table[index].opendir(pathname);
    if (state == 1) {
        debug("          open dir success!");
    } else {
		DisErrorInfo(state);
    }    
}

PUBLIC int do_vcreatedir(char *path) {
    int state;

    int pathlen = strlen(path);
    char pathname[MAX_PATH];
    
    strcpy(pathname,path);
    pathname[pathlen] = 0;

    int index;
    index = (int)(pathname[1]-'0');

    for(int j=0;j<= pathlen-3;j++)
    {
        pathname[j] = pathname[j+3];
    }
    state = f_op_table[index].createdir(pathname);
    if (state == 1) {
        debug("          create dir success!");
    } else {
		DisErrorInfo(state);
    }    
}

PUBLIC int do_vdeletedir(char *path) {
    int state;
    int pathlen = strlen(path);
    char pathname[MAX_PATH];
    
    strcpy(pathname,path);
    pathname[pathlen] = 0;

    int index;
    index = (int)(pathname[1]-'0');

    for(int j=0;j<= pathlen-3;j++)
    {
        pathname[j] = pathname[j+3];
    }
    state = f_op_table[index].deletedir(pathname);
    if (state == 1) {
        debug("          delete dir success!");
    } else {
		DisErrorInfo(state);
    }   
}


PRIVATE int strcmp(const char * s1, const char *s2)
{
	if ((s1 == 0) || (s2 == 0)) { /* for robustness */
		return (s1 - s2);
	}

	const char * p1 = s1;
	const char * p2 = s2;

	for (; *p1 && *p2; p1++,p2++) {
		if (*p1 != *p2) {
			break;
		}
	}

	return (*p1 - *p2);
}