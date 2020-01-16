/**********************************************************
*	vfs.h       //added by mingxuan 2019-5-17
***********************************************************/

#define NR_DEV 10
#define DEV_NAME_LEN 15
#define NR_fs 3

//#define FILE_MAX_LEN 512*4	//最大长度为4个扇区
#define FILE_MAX_LEN 512*16		//最大长度为16个扇区(8KB)

//设备表
struct device{
    char * dev_name; //设备名
    struct file_op* op;          //指向操作表的一项
    int  dev_num;                //设备号
};

PUBLIC int sys_open(void *uesp);
PUBLIC int sys_close(void *uesp);
PUBLIC int sys_read(void *uesp);
PUBLIC int sys_write(void *uesp);
PUBLIC int sys_lseek(void *uesp);
PUBLIC int sys_unlink(void *uesp);
PUBLIC int sys_create(void *uesp);
PUBLIC int sys_delete(void *uesp);
PUBLIC int sys_opendir(void *uesp);
PUBLIC int sys_createdir(void *uesp);
PUBLIC int sys_deletedir(void *uesp);

PUBLIC int do_vopen(const char *path, int flags);
PUBLIC int do_vclose(int fd);
PUBLIC int do_vread(int fd, char *buf, int count);
PUBLIC int do_vwrite(int fd, const char *buf, int count);
PUBLIC int do_vunlink(const char *path);
PUBLIC int do_vlseek(int fd, int offset, int whence);
PUBLIC int do_vcreate(char *pathname);
PUBLIC int do_vdelete(char *path);
PUBLIC int do_vopendir(char *dirname);
PUBLIC int do_vcreatedir(char *dirname);
PUBLIC int do_vdeletedir(char *dirname);

PUBLIC void init_vfs();
int sys_CreateFile(void *uesp);
int sys_DeleteFile(void *uesp);
int sys_OpenFile(void *uesp);
int sys_CloseFile(void *uesp);
int sys_WriteFile(void *uesp);
int sys_ReadFile(void *uesp);
int sys_OpenDir(void *uesp);
int sys_CreateDir(void *uesp);
int sys_DeleteDir(void *uesp);
int sys_ListDir(void *uesp);

//文件系统的操作函数
struct file_op{
    int (*create)   (const char*);
	int (*open)    (const char* ,int);
	int (*close)   (int);
	int (*read)    (int,void * ,int);
	int (*write)   (int ,const void* ,int);
	int (*lseek)   (int ,int ,int);
	int (*unlink)  (const char*);
    int (*delete) (const char*);
	int (*opendir) (const char *);
	int (*createdir) (const char *);
	int (*deletedir) (const char *);
};