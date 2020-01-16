/*
 * To test if new kernel features work normally, and if old features still 
 * work normally with new features added.
 * added by xw, 18/4/27
 */
#include "type.h"
#include "const.h"
#include "protect.h"
#include "string.h"
#include "proc.h"
#include "global.h"
#include "proto.h"
#include "fs.h"

/*======================================================================*
                         Interrupt Handling Test
added by xw, 18/4/27
 *======================================================================*/
	/*
void TestA()
{
	int i, j;
	while (1)
	{
		disp_str("A ");

		i = 100;
		while(--i){
			j = 1000;
			while(--j){}
		}
	}
}

void TestB()
{
	int i, j;
	while (1)
	{
		disp_str("B ");

		i = 100;
		while(--i){
			j = 1000;
			while(--j){}
		}
	}
}

void TestC()
{
	int i, j;
	while (1)
	{
		disp_str("C ");

		i = 100;
		while(--i){
			j = 1000;
			while(--j){}
		}
	}
}

void initial()
 {
	int i, j;
	while (1)
	{
		disp_str("I ");

		i = 100;
		while(--i){
			j = 1000;
			while(--j){}
		}
	}
 }
//	*/

/*======================================================================*
                         Exception Handling Test
added by xw, 18/12/19
 *======================================================================*/
	/*
void TestA()
{
	int i, j;
	while (1)
	{
		disp_str("A ");

		i = 100;
		while(--i){
			j = 1000;
			while(--j){}
		}
		
		//generates an Undefined opcode(#UD) exception, without error code
		//UD2 is provided by Intel to explicitly generate an invalid opcode exception.
//		asm volatile ("ud2");
		
		//generates a General Protection(#GP) exception, with error code
		//the privilege level of a procedure must be 0 to execute the HLT instruction
//		asm volatile ("hlt");
		
		//generates a Divide Error(#DE) exception, without error code
		//calculate a / b
		int a, b;
		a = 10, b = 0;
		asm volatile ("mov %0, %%eax\n\t"
					  "div %1\n\t"
					  : 
					  : "r"(a), "r"(b)
					  : "eax");		
		
	}
}

void TestB()
{
	int i, j;
	while (1)
	{
		disp_str("B ");

		i = 100;
		while(--i){
			j = 1000;
			while(--j){}
		}
	}
}

void TestC()
{
	int i, j;
	while (1)
	{
		disp_str("C ");

		i = 100;
		while(--i){
			j = 1000;
			while(--j){}
		}
	}
}

void initial()
 {
	int i, j;
	while (1)
	{
		disp_str("I ");

		i = 100;
		while(--i){
			j = 1000;
			while(--j){}
		}
	}
 }
//	*/

/*======================================================================*
                        Ordinary System Call Test
added by xw, 18/4/27
 *======================================================================*/
	/*
void TestA()
{
	int i, j;
	while (1)
	{
		disp_str("A ");
		milli_delay(100);
	}
}

void TestB()
{
	int i, j;
	while (1)
	{
		disp_str("B ");
		milli_delay(100);
	}
}

void TestC()
{
	int i, j;
	while (1)
	{
		disp_str("C ");
		milli_delay(100);
	}
}

void initial()
 {
	int i, j;
	while (1)
	{
		disp_str("I ");
		milli_delay(100);
	}
 }
//	*/
	 

/*======================================================================*
                          Kernel Preemption Test
added by xw, 18/4/27
 *======================================================================*/
	/*
void TestA()
{
	int i, j;
	while (1)
	{
		disp_str("A ");
		print_E();
		milli_delay(100);
	}
}

void TestB()
{
	int i, j;
	while (1)
	{
		disp_str("B ");
		print_F();
		milli_delay(100);
	}
}

void TestC()
{
	int i, j;
	while (1)
	{
		disp_str("C ");
		milli_delay(100);
	}
}

void initial()
 {
	int i, j;
	while (1)
	{
		disp_str("I ");
		milli_delay(100);
	}
 }
//	*/

/*======================================================================*
                          Syscall Yield Test
added by xw, 18/4/27
 *======================================================================*/
	/*
void TestA()
{
	int i;
	while (1) 
	{
		disp_str("A( ");
		yield();
		disp_str(") ");
		milli_delay(100);
	} 
}

void TestB()
{
	int i, j;
	while (1)
	{
		disp_str("B ");
		milli_delay(100);
	}
}

void TestC()
{
	int i, j;
	while (1)
	{
		disp_str("C ");
		milli_delay(100);
	}
}

void initial()
 {
	int i, j;
	while (1)
	{
		disp_str("I ");
		milli_delay(100);
	}
 }
//	*/

/*======================================================================*
                          Syscall Sleep Test
added by xw, 18/4/27
 *======================================================================*/
	/*
void TestA()
{
	int i;
	while (1) 
	{
		disp_str("A( ");
		disp_str("[");
		disp_int(ticks);
		disp_str("] ");
		sleep(5);
		disp_str("[");
		disp_int(ticks);
		disp_str("] ");
		disp_str(") ");
		milli_delay(100);
	} 
}

void TestB()
{
	int i, j;
	while (1)
	{
		disp_str("B ");
		milli_delay(100);
	}
}

void TestC()
{
	int i, j;
	while (1)
	{
		disp_str("C ");
		milli_delay(100);
	}
}

void initial()
 {
	int i, j;
	while (1)
	{
		disp_str("I ");
		milli_delay(100);
	}
 }
//	*/

/*======================================================================*
                          User Process Test
added by xw, 18/4/27
 *======================================================================*/
/* You should also enable the feature you want to test in init.c */
/*
void TestA()
{
	int i, j;
	while (1)
	{
		disp_str("A ");
		milli_delay(100);
	}
}

void TestB()
{
	int i, j;
	while (1)
	{
		disp_str("B ");
		milli_delay(100);
	}
}

void TestC()
{
	int i, j;
	while (1)
	{
		disp_str("C ");
		milli_delay(100);
	}
}
*/

/**
 * @struct posix_tar_header
 * Borrowed from GNU `tar'
 */
//added by mingxuan 2019-5-18
struct posix_tar_header
{				/* byte offset */
	char name[100];		/*   0 */
	char mode[8];		/* 100 */
	char uid[8];		/* 108 */
	char gid[8];		/* 116 */
	char size[12];		/* 124 */
	char mtime[12];		/* 136 */
	char chksum[8];		/* 148 */
	char typeflag;		/* 156 */
	char linkname[100];	/* 157 */
	char magic[6];		/* 257 */
	char version[2];	/* 263 */
	char uname[32];		/* 265 */
	char gname[32];		/* 297 */
	char devmajor[8];	/* 329 */
	char devminor[8];	/* 337 */
	char prefix[155];	/* 345 */
	/* 500 */
};

/*****************************************************************************
 *                                untar
 * added by mingxuan 2019-5-18
 *****************************************************************************/
/**
 * Extract the tar file and store them.
 * 
 * @param filename The tar file.
 *****************************************************************************/
PRIVATE void untar(const char * filename)
{
	printf("[extract %s \n", filename);
	//int fd = open(filename, O_RDWR);	//deleted by mingxuan 2019-5-20
	int fd = do_vopen(filename, O_RDWR);//modified by mingxuan 2019-5-20
	// assert(fd != -1);

	char buf[512 * 16];
	int chunk = sizeof(buf);
	int i = 0;
	int bytes = 0;

	while (1) {
		//bytes = read(fd, buf, 512);	//deleted by mingxuan 2019-5-21
		bytes = do_vread(fd, buf, 512); //modified by mingxuan 2019-5-21
		// assert(bytes == 512); /* size of a TAR file must be multiple of 512 */
		if (buf[0] == 0) {
			if (i == 0)
				printf("    need not unpack the file.\n");
			break;
		}
		i++;

		struct posix_tar_header * phdr = (struct posix_tar_header *)buf;

		/* calculate the file size */
		char * p = phdr->size;
		int f_len = 0;
		while (*p)
			f_len = (f_len * 8) + (*p++ - '0'); /* octal */

		int bytes_left = f_len;
		char full_name[30] = "orange/";
		strcat(full_name,phdr->name);
		//int fdout = open(full_name, O_CREAT | O_RDWR );	//deleted by mingxuan 2019-5-20
		int fdout = do_vopen(full_name, O_CREAT | O_RDWR );	//modified by mingxuan 2019-5-20
		if (fdout == -1) {
			printf("    failed to extract file: %s\n", phdr->name);
			printf(" aborted]\n");
			//close(fd);	//deleted by mingxuan 2019-5-20
			do_vclose(fd);	//modified by mingxuan 2019-5-20
			return;
		}
		printf("    %s \n", phdr->name);	//deleted by mingxuan 2019-5-22
		while (bytes_left) {
			int iobytes = min(chunk, bytes_left);
			
			//read(fd, buf, ((iobytes - 1) / 512 + 1) * 512);	//deleted by mingxuan 2019-5-21
			do_vread(fd, buf, ((iobytes - 1) / 512 + 1) * 512);	//modified by mingxuan 2019-5-21
			
			//bytes = write(fdout, buf, iobytes);	//deleted by mingxuan 2019-5-21
			bytes = do_vwrite(fdout, buf, iobytes); //modified by mingxuan 2019-5-21
			//bytes = real_write(fdout, buf, iobytes);

			//测试
			//real_lseek(fd,0,SEEK_SET);
			//do_vread(fdout, buf, iobytes);
			//real_read(fdout, buf, iobytes);

			// assert(bytes == iobytes);
			bytes_left -= iobytes;
		}
		//close(fdout);		//deleted by mingxuan 2019-5-20
		do_vclose(fdout);	//modified by mingxuan 2019-5-20
	}

	if (i) {
		//lseek(fd, 0, SEEK_SET);	//deleted by mingxuan 2019-5-20
		do_vlseek(fd, 0, SEEK_SET); //modified by mingxuan 2019-5-20
		buf[0] = 0;
		//bytes = write(fd, buf, 1);//deleted by mingxuan 2019-5-20
		bytes = do_vwrite(fd, buf, 1);	//modified by mingxuan 2019-5-20
	}

	//close(fd);	//deleted by mingxuan 2019-5-21
	do_vclose(fd);	//modified by mingxuan 2019-5-21

	printf(" done, %d files extracted]\n", i);
}


void initial()
{

	/***************************************
	 * exec直接加载init,bin
	****************************************/
	/*
	int fd=0;
	//fd = exec("init/init.bin");
	//char filename[] = "orange/test34.txt";
	//fd = do_vopen(filename, O_CREAT | O_RDWR);

	int stdin = do_vopen("dev_tty0",O_RDWR);
	int stdout= do_vopen("dev_tty0",O_RDWR);
	int stderr= do_vopen("dev_tty0",O_RDWR);

	printf("Hello world!\n");

	#ifdef INSTALL_TAR
		char full_name[30]="orange/";;
		printf("untar:%s\n",full_name);
		strcat(full_name,INSTALL_FILENAME);
		untar(full_name);
	#endif

	//printf("exec begin\n");
	
	close(stdin);
	close(stdout);
	close(stderr);
	
	exec("orange/init.bin");
	//exec("orange/app.tar");
	
	while(1);
	*/
	
	/***************************************
	 * 		exec加载单shell_0.bin
	 * added by mingxuan 2019-5-22
	****************************************/
	
	int stdin = do_vopen("dev_tty0",O_RDWR);
	int stdout= do_vopen("dev_tty0",O_RDWR);
	int stderr= do_vopen("dev_tty0",O_RDWR);

	//untar(INSTALL_FILENAME);
	//modified by mingxuan 2019-5-21
	char full_name[30]="orange/";;
	printf("untar:%s\n",full_name);
	strcat(full_name,INSTALL_FILENAME);
	untar(full_name);

	do_vclose(stdin);
	do_vclose(stdout);
	do_vclose(stderr);

	exec("orange/shell_0.bin");
	

	/***************************************
	 * 		exec加载多shell(目前还未实现)
	 * added by mingxuan 2019-5-22
	****************************************/
	/*
	int pid = fork();
	if(pid==0){
		int j = 0x00ffffff;
		int k = j << 1;
		int i = fork();
		if(i==0){
			while(j--);
			//exec("shell_1.bin");
			exec("orange/shell_1.bin");
			while(1);
		}
		else{
			while(k--);
			//exec("shell_0.bin");
			exec("orange/shell_0.bin");
			while(1);
		}
	}else{
		//exec("shell_2.bin");
		exec("orange/shell_2.bin");
		while(1);
	}
	*/

	while(1);
	
}
//	*/

/*======================================================================*
                          File System Test
added by xw, 18/5/26
 *======================================================================*/
	/*
void TestA()
{	
	//while (1) {}
	
	//manipulate /blah
	//if /blah exists, open
	//if /blah doesn't exist, open+write+close
	int fd;
	int i, n;
	char filename[MAX_FILENAME_LEN+1] = "blah";
	const char bufw[] = "abcde";
	const int rd_bytes = 4;
	char bufr[5];

	disp_str("(TestA)");
	fd = open(filename, O_CREAT | O_RDWR);	
	
	if(fd != -1) {
		disp_str("File created: ");
		disp_str(filename);
		disp_str(" (fd ");
		disp_int(fd);
		disp_str(")\n");	
		
		n = write(fd, bufw, strlen(bufw));
		if(n != strlen(bufw)) {
			disp_str("Write error\n");
		}
		
		close(fd);
	}
	
	while (1) {
	}
}


void TestB()
{	
	//while (1) {}
	
	//manipulate /blah, open+lseek+read+close
	int fd, n;
	const int rd_bytes = 4;
	char bufr[5];
	char filename[MAX_FILENAME_LEN+1] = "blah";

	disp_str("(TestB)");
	fd = open(filename, O_RDWR);
	disp_str("       ");
	disp_str("File opened. fd: ");
	disp_int(fd);
	disp_str("\n");

	disp_str("(TestB)");
	int lseek_status = lseek(fd, 1, SEEK_SET);
	disp_str("Return value of lseek is: ");
	disp_int(lseek_status);
	disp_str("  \n");

	disp_str("(TestB)");
	n = read(fd, bufr, rd_bytes);
	if(n != rd_bytes) {
		disp_str("Read error\n");
	}
	bufr[n] = 0;
	disp_str("Bytes read: ");
	disp_str(bufr);
	disp_str("\n");

	close(fd);

	while(1){

	}
}

void TestC()
{
	//while (1) {}
	
	//manipulate /blah, open+lseek+read+close
	int fd, n;
	const int rd_bytes = 3;
	char bufr[4];
	char filename[MAX_FILENAME_LEN+1] = "blah";
	
	disp_str("(TestC)");
	fd = open(filename, O_RDWR);
	disp_str("       ");
	disp_str("File opened. fd: ");
	disp_int(fd);
	disp_str("\n");

	disp_str("(TestC)");
	int lseek_status = lseek(fd, 1, SEEK_SET);
	disp_str("Return value of lseek is: ");
	disp_int(lseek_status);
	disp_str("  \n");

	disp_str("(TestC)");
	n = read(fd, bufr, rd_bytes);
	if(n != rd_bytes) {
		disp_str("Read error\n");
	}
	bufr[n] = 0;
	disp_str("bytes read: ");
	disp_str(bufr);
	disp_str("\n");

	close(fd);
	
	while(1){
	}
}

void initial()
{
	//while (1) {}
	
	int i, fd;
	char* filenames[] = {"/foo", "/bar", "/baz"};
	char* rfilenames[] = {"/bar", "/foo", "/baz", "/dev_tty0"};
	
	//open+close
	for (i = 0; i < sizeof(filenames) / sizeof(filenames[0]); i++) {
		disp_str("(Initial)");
		fd = open(filenames[i], O_CREAT | O_RDWR);
		if(fd != -1) {
			disp_str("File created: ");
			disp_str(filenames[i]);
			disp_str(" (fd ");
			disp_int(fd);
			disp_str(")\n");

			close(fd);
		}
	}

	//unlink
	for (i = 0; i < sizeof(rfilenames) / sizeof(rfilenames[0]); i++) {
		disp_str("(Initial)");
		if (unlink(rfilenames[i]) == 0) {
			disp_str("File removed: ");
			disp_str(rfilenames[i]);
			disp_str("\n");
		}
		else {
			disp_str("         ");
			disp_str("Failed to remove file: ");
			disp_str(rfilenames[i]);
			disp_str("\n");
		}
	}
	
	while (1){
	}
}
//	*/

/*======================================================================*
                          File System Test 2
to test interrupt_wait_sched() used in hd.c
interrupt_wait_sched() is not used currently, because it can only support
one process to access hd disk, as you can see below.
added by xw, 18/8/16
 *======================================================================*/
	/*
void TestA()
{
	int i, j;
	while (1)
	{
		disp_str("A ");
		milli_delay(100);
	}
}

void TestB()
{	
	//while (1) {}
	
	int fd, n;
	char bufr[5];
	char bufw[5] = "mmnn";
	char filename[MAX_FILENAME_LEN+1] = "blah";

	disp_str("(TestB)");
	fd = open(filename, O_RDWR);
	disp_str("       ");
	disp_str("File opened. fd: ");
	disp_int(fd);
	disp_str("\n");

	disp_str("(TestB)");
	lseek(fd, 1, SEEK_SET);
	n = write(fd, bufw, 4);
	if(n != 4) {
		disp_str("Write error\n");
	}
	bufr[4] = 0;
	disp_str("Bytes write: ");
	disp_str(bufw);
	disp_str("\n");
	
	disp_str("(TestB)");
	lseek(fd, 1, SEEK_SET);
	n = read(fd, bufr, 4);
	if(n != 4) {
		disp_str("Read error\n");
	}
	bufr[4] = 0;
	disp_str("Bytes read: ");
	disp_str(bufr);
	disp_str("\n");

	close(fd);

	while(1){

	}
}

void TestC()
{
	int i, j;
	while (1)
	{
		disp_str("C ");
		milli_delay(100);
	}
}

void initial()
 {
	int i, j;
	while (1)
	{
		disp_str("I ");
		milli_delay(100);
	}
 }
//	*/
