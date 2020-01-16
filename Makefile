#########################
# Makefile for Orange'S #
#########################

# Entry point of Orange'S
# It must have the same value with 'KernelEntryPointPhyAddr' in load.inc!
#############edit by visual 2016.5.10####
ENTRYPOINT	= 0xC0030400

# Offset of entry point in kernel file
# It depends on ENTRYPOINT
ENTRYOFFSET	=   0x400

# Programs, flags, etc.
ASM		= nasm
DASM	= ndisasm
CC		= gcc
LD		= ld
AR		= ar

# ASMBFLAGS	= -I boot/include/	#deleted by mingxuan 2019-5-17
ASMBFLAGS	= -I boot/floppy/include/	#added by mingxuan 2019-5-17
ASMBFLAGS_grub  = -I boot/grub/include/ #added by mingxuan 2019-5-17
ASMBFLAGS_mbr   = -I boot/mbr/include/	#added by mingxuan 2019-5-17
ASMKFLAGS	= -I include/ -f elf
DASMFLAGS	= -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)
ARFLAGS		= rcs

# CFLAGS		= -I include/ -c -fno-builtin -fno-stack-protector
# CFLAGS		= -I include/ -m32 -c -fno-builtin -fno-stack-protector
#modified by xw
CFLAGS		= -I include/ -m32 -c -fno-builtin -fno-stack-protector -Wall -Wextra -g
CFLAGS_app	= -I include/ -m32 -c -fno-builtin -fno-stack-protector -Wall -Wextra -g

# LDFLAGS		= -s -Ttext $(ENTRYPOINT)
# LDFLAGS		= -m elf_i386 -s -Ttext $(ENTRYPOINT)
#generate map file. added by xw
LDFLAGS_kernel	= -m elf_i386 -s -Ttext $(ENTRYPOINT) -Map misc/kernel.map
LDFLAGS_init	= -m elf_i386 -s -Map init/init.map
#discard -s, so keep symbol information that gdb can use. added by xw
LDFLAGS_kernel_gdb	= -m elf_i386 -Ttext $(ENTRYPOINT)
LDFLAGS_init_gdb	= -m elf_i386

# This Program
# ORANGESBOOT	= boot/boot.bin boot/loader.bin	#deleted by mingxuan 2019-5-17
ORANGESBOOT	= boot/floppy/boot.bin boot/floppy/loader.bin \
			  boot/grub/boot.bin boot/grub/loader.bin \
			  boot/mbr/mbr.bin boot/mbr/boot.bin boot/mbr/loader.bin #modified by mingxuan 2019-5-17
ORANGESKERNEL	= kernel.bin
ORANGESINIT	= init/init.bin
OBJS		= kernel/kernel.o kernel/syscall.o kernel/start.o kernel/main.o kernel/clock.o\
			kernel/i8259.o kernel/global.o kernel/protect.o kernel/proc.o\
			lib/kliba.o lib/klib.o lib/stringc.o lib/stringasm.o kernel/syscallc.o kernel/memman.o kernel/pagetbl.o	\
			kernel/elf.o kernel/file.o kernel/exec.o kernel/fork.o kernel/pthread.o \
			kernel/ktest.o kernel/testfunc.o kernel/fs.o kernel/hd.o \
			kernel/spinlock.o kernel/fat32.o kernel/base.o kernel/assist.o kernel/vfs.o \
			kernel/list.o kernel/swap.o \
			kernel/keyboard.o kernel/tty.o kernel/shell.o kernel/console.o lib/ulib.a #added by mingxuan 2019-5-19
OBJSINIT	= init/init.o init/initstart.o lib/ulib.a
#OBJSULIB 	= lib/string.o kernel/syscall.o	#deleted by mingxuan 2019-5-19
OBJSULIB 	= lib/string.o kernel/syscall.o lib/printf.o lib/vsprintf.o lib/scanf.o #added by mingxuan 2019-5-19
DASMOUTPUT	= kernel.bin.asm
#added by xw
GDBBIN = kernel.gdb.bin init/init.gdb.bin

# All Phony Targets
# .PHONY : everything final image clean realclean disasm all buildimg tags 	#deleted by mingxuan 2019-5-19
.PHONY : everything final image clean realclean disasm all buildimg tags install_elf  #modified by mingxuan 2019-5-19

# Default starting position
nop :
	@echo "why not \`make image' huh? :)"

everything : $(ORANGESBOOT) $(ORANGESKERNEL) $(ORANGESINIT) $(GDBBIN)

all : realclean everything

# image : realclean everything clean buildimg
# image : everything buildimg tags	# deleted by mingxuan 2019-5-17
# image : everything buildimg_mbr tags #deleted by mingxuan 2019-5-19
# image : realclean everything clean buildimg tags # deleted by mingxuan 2019-5-17
# image : realclean everything clean buildimg_mbr tags   # modified by mingxuan 2019-5-17	

# image : everything buildimg_mbr tags install_elf	#modified by mingxuan 2019-5-21
# image : everything buildimg_mbr tags install_user
# image : realclean everything clean buildimg_mbr tags install_elf
# image : realclean everything clean buildimg_mbr tags install_user
#image : realclean everything buildimg_mbr tags install_elf
image : realclean everything buildimg_mbr tags install_user

# added by mingxuan 2019-5-19
install_elf:
	cd ./init && \
	make 

# added by mingxuan 2019-5-19
install_user:
	cd ./user && \
	make install

clean :
# 	rm -f $(OBJS) $(OBJSINIT) #modified by mingxuan 2019-5-23
	rm -f $(OBJS) $(OBJSINIT) $(OBJSULIB) 

realclean :
#	rm -f $(OBJS) $(OBJSINIT) $(ORANGESBOOT) $(ORANGESKERNEL) $(ORANGESINIT) $(GDBBIN)
	rm -f $(OBJS) $(OBJSINIT) $(ORANGESBOOT) $(ORANGESKERNEL) $(ORANGESINIT) $(OBJSULIB) $(GDBBIN)

disasm :
	$(DASM) $(DASMFLAGS) $(ORANGESKERNEL) > $(DASMOUTPUT)

# We assume that "a.img" exists in current folder
buildimg :
#	dd if=boot/boot.bin of=a.img bs=512 count=1 conv=notrunc
	dd if=boot/floppy/boot.bin of=a.img bs=512 count=1 conv=notrunc	# modified by mingxuan 2019-5-17
	sudo mount -o loop a.img /mnt/floppy/
#	sudo cp -fv boot/loader.bin /mnt/floppy/
	sudo cp -fv boot/floppy/loader.bin /mnt/floppy/ # modified by mingxuan 2019-5-17
	sudo cp -fv kernel.bin /mnt/floppy
	sudo cp -fv init/init.bin /mnt/floppy
#	sudo cp -fv command/echo.bin /mnt/floppy
	sudo umount /mnt/floppy

# added by mingxuan 2019-5-17
buildimg_grub:
	dd if=boot/grub/boot.bin of=c.img bs=1 count=512 seek=1048576 conv=notrunc
	dd if=boot/grub/boot.bin of=c.img bs=1 count=512 seek=1051648 conv=notrunc
	#sudo kpartx -av c.img
	#sudo mount -o loop /dev/mapper/loop0p1 iso/
	sudo losetup -P /dev/loop0 c.img
	sudo mount /dev/loop0p1 iso/
	sudo cp -fv boot/grub/grub.cfg iso/grub
	sudo cp -fv boot/grub/loader.bin iso/
	sudo cp -fv kernel.bin iso/
	sudo cp -fv init/init.bin iso/
	sudo umount iso/
	#sudo kpartx -d c.img
	sudo losetup -d /dev/loop0

# added by mingxuan 2019-5-17
buildimg_mbr:
	dd if=boot/mbr/mbr.bin of=b.img bs=1 count=446 conv=notrunc
	dd if=boot/mbr/boot.bin of=b.img bs=1 count=512 seek=1048576 conv=notrunc
	dd if=boot/mbr/boot.bin of=b.img bs=1 count=512 seek=1051648 conv=notrunc
	sudo kpartx -asv b.img

	# sudo mkfs.vfat -F 32 /dev/mapper/loop0p1

	sudo mount -o loop /dev/mapper/loop0p1 iso/
	#sudo losetup -P /dev/loop0 b.img
	#sudo mount /dev/loop0p1 iso/
	sudo cp -fv boot/mbr/grub/grub.cfg iso/grub
	sudo cp -fv boot/mbr/loader.bin iso/
	sudo cp -fv kernel.bin iso/
	# sudo cp -fv init/init.bin iso/	#exec is real now, no need to use cp init #deleted by mingxuan 2019-5-23
	sudo umount iso/
	sudo kpartx -d b.img
	#sudo losetup -d /dev/loop0

# generate tags file. added by xw, 19/1/2
tags :
	ctags -R	

archive :
	git archive --prefix=

# deleted by mingxuan 2019-5-17
# boot/boot.bin : boot/boot.asm boot/include/load.inc boot/include/fat12hdr.inc
#	$(ASM) $(ASMBFLAGS) -o $@ $<

# deleted by mingxuan 2019-5-17
# boot/loader.bin : boot/loader.asm boot/include/load.inc boot/include/fat12hdr.inc boot/include/pm.inc
#	$(ASM) $(ASMBFLAGS) -o $@ $<

# added by mingxuan 2019-5-17
boot/floppy/boot.bin : boot/floppy/boot.asm boot/floppy/include/load.inc boot/floppy/include/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

# added by mingxuan 2019-5-17
boot/floppy/loader.bin : boot/floppy/loader.asm boot/floppy/include/load.inc boot/floppy/include/fat12hdr.inc boot/floppy/include/pm.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

# added by mingxuan 2019-5-17
boot/grub/boot.bin : boot/grub/boot.asm 
	$(ASM) $(ASMBFLAGS_grub) -o $@ $<

# added by mingxuan 2019-5-17
boot/grub/loader.bin : boot/grub/loader.asm boot/grub/include/loader.inc boot/grub/include/fat32.inc boot/grub/include/pm.inc
	$(ASM) $(ASMBFLAGS_grub) -o $@ $<

# added by mingxuan 2019-5-17
boot/mbr/mbr.bin : boot/mbr/mbr.asm 
	$(ASM) $(ASMBFLAGS_mbr) -o $@ $<

# added by mingxuan 2019-5-17
boot/mbr/boot.bin : boot/mbr/boot.asm 
	$(ASM) $(ASMBFLAGS_mbr) -o $@ $<

# added by mingxuan 2019-5-17
boot/mbr/loader.bin : boot/mbr/loader.asm boot/mbr/include/loader.inc boot/mbr/include/fat32.inc boot/mbr/include/pm.inc
	$(ASM) $(ASMBFLAGS_mbr) -o $@ $<

$(ORANGESKERNEL) : $(OBJS)
#	$(LD) $(LDFLAGS) -o $(ORANGESKERNEL) $(OBJS)
#modified by xw, 18/6/10
#	$(LD) $(LDFLAGS) -Map kernel.map -o $(ORANGESKERNEL) $(OBJS)
#modified by xw, 18/6/12
	$(LD) $(LDFLAGS_kernel) -o $(ORANGESKERNEL) $(OBJS)
	
$(ORANGESINIT) : $(OBJSINIT)
#	$(LD) -s -o $(ORANGESINIT) $(OBJSINIT)
#	$(LD) -m elf_i386 -s -o $(ORANGESINIT) $(OBJSINIT)
#modified by xw, 18/6/11
#	$(LD) -m elf_i386 -s -Map init/init.map -o $(ORANGESINIT) $(OBJSINIT)
#modified by xw, 18/6/12
	$(LD) $(LDFLAGS_init) -o $(ORANGESINIT) $(OBJSINIT)

#added by xw
kernel.gdb.bin : $(OBJS)
	$(LD) $(LDFLAGS_kernel_gdb) -o $@ $(OBJS)
init/init.gdb.bin : $(OBJSINIT)
	$(LD) $(LDFLAGS_init_gdb) -o $@ $(OBJSINIT)
	
kernel/kernel.o : kernel/kernel.asm include/sconst.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<

kernel/syscall.o : kernel/syscall.asm include/sconst.inc
	$(ASM) $(ASMKFLAGS) -o $@ $<

kernel/start.o: kernel/start.c include/type.h include/const.h include/protect.h include/string.h include/proc.h include/proto.h \
			include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/main.o: kernel/main.c include/type.h include/const.h include/protect.h include/string.h include/proc.h include/proto.h \
			include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/clock.o: kernel/clock.c
	$(CC) $(CFLAGS) -o $@ $<

kernel/i8259.o: kernel/i8259.c include/type.h include/const.h include/protect.h include/proto.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/global.o: kernel/global.c include/type.h include/const.h include/protect.h include/proc.h \
			include/global.h include/proto.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/protect.o: kernel/protect.c include/type.h include/const.h include/protect.h include/proc.h include/proto.h \
			include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/proc.o: kernel/proc.c
	$(CC) $(CFLAGS) -o $@ $<

lib/klib.o: lib/klib.c include/type.h include/const.h include/protect.h include/string.h include/proc.h include/proto.h \
			include/global.h
	$(CC) $(CFLAGS) -o $@ $<

lib/kliba.o : lib/kliba.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

# deleted by mingxuan 2019-5-19
# lib/string.o : lib/string.asm
#	$(ASM) $(ASMKFLAGS) -o $@ $<

# stringasm.o	added by mingxuan 2019-5-19
lib/stringasm.o: lib/string.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<

# stringc.o	added by mingxuan 2019-5-19
lib/stringc.o:	lib/string.c
	$(CC) $(CFLAGS) -o $@ $<

# string.o	added by mingxuan 2019-5-19
lib/string.o : lib/stringasm.o lib/stringc.o
	$(LD) -m elf_i386 -r -o $@ $<

kernel/syscallc.o: kernel/syscallc.c include/type.h include/const.h include/protect.h include/proto.h \
			include/string.h include/proc.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<	

kernel/memman.o: kernel/memman.c /usr/include/stdc-predef.h include/memman.h include/type.h include/const.h include/protect.h \
 			include/proto.h include/string.h include/proc.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<
	
kernel/pagetbl.o: kernel/pagetbl.c include/type.h include/const.h include/protect.h include/proto.h include/string.h \
			include/proc.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<	

lib/ulib.a:  $(OBJSULIB)
	$(AR) $(ARFLAGS) -o $@  $(OBJSULIB)
	
init/init.o: init/init.c include/stdio.h
	$(CC) $(CFLAGS_app) -o $@ $<
	
init/initstart.o: init/initstart.asm 
	$(ASM) $(ASMKFLAGS) -o $@ $<
	
kernel/elf.o: kernel/elf.c /usr/include/stdc-predef.h include/type.h include/const.h include/protect.h \
			include/proto.h include/string.h include/proc.h include/global.h include/elf.h
	$(CC) $(CFLAGS) -o $@ $<
	
kernel/file.o: kernel/file.c /usr/include/stdc-predef.h include/type.h include/const.h include/protect.h \
			include/proto.h include/string.h include/proc.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<
	
kernel/exec.o: kernel/exec.c /usr/include/stdc-predef.h include/type.h include/const.h include/protect.h \
			include/proto.h include/string.h include/proc.h include/global.h include/elf.h
	$(CC) $(CFLAGS) -o $@ $<
	
kernel/fork.o: kernel/fork.c /usr/include/stdc-predef.h include/type.h include/const.h include/protect.h \
			include/proto.h include/string.h include/proc.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<
	
kernel/pthread.o: kernel/pthread.c /usr/include/stdc-predef.h include/type.h include/const.h include/protect.h \
			include/proto.h include/string.h include/proc.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<

kernel/ktest.o: kernel/ktest.c include/type.h include/const.h include/protect.h include/string.h include/proc.h \
			include/proto.h include/global.h
#	$(CC) $(CFLAGS) -o $@ $<	#modified by mingxuan 2019-5-19
	$(CC) $(CFLAGS) \
		-D INSTALL_FILENAME='"$(INSTALL_FILENAME)"' \
		-D $(INSTALL_TYPE) \
		-o $@ $<

kernel/testfunc.o: kernel/testfunc.c include/type.h include/const.h include/protect.h include/string.h include/proc.h \
			include/proto.h include/global.h
	$(CC) $(CFLAGS) -o $@ $<
	
# fs.o and hd.o; added by xw, 18/5/25
kernel/fs.o: kernel/fs.c include/type.h include/const.h include/protect.h include/string.h include/proc.h \
			include/proto.h include/global.h include/fs_const.h include/fs.h include/hd.h include/fs_misc.h
#	$(CC) $(CFLAGS) -o $@ $<	#modified by mingxuan 2019-5-19
	$(CC) $(CFLAGS) \
		-D INSTALL_FILENAME='"$(INSTALL_FILENAME)"' \
		-D INSTALL_NR_SECTORS=$(INSTALL_NR_SECTORS) \
		-D INSTALL_START_SECTOR=$(INSTALL_START_SECTOR) \
		-D $(INSTALL_TYPE) \
		-o $@ $< 

kernel/hd.o: kernel/hd.c include/type.h include/const.h include/protect.h include/string.h include/proc.h \
			include/proto.h include/global.h include/fs_const.h include/fs.h include/hd.h include/fs_misc.h
	$(CC) $(CFLAGS) -o $@ $<

# spinlock.o	added by mingxuan 2019-1-5
kernel/spinlock.o: kernel/spinlock.c include/spinlock.h
	$(CC) -masm=intel $(CFLAGS) -o $@ $<

# fat32.o	added by mingxuan 2019-5-17
kernel/fat32.o: kernel/fat32.c include/type.h include/const.h include/protect.h include/string.h include/proc.h \
			include/proto.h include/global.h include/fs_const.h include/fs.h include/hd.h include/fs_misc.h \
			include/fat32.h
	$(CC) $(CFLAGS) -o $@ $<

# base.o	added by mingxuan 2019-5-17
kernel/base.o: kernel/base.c include/type.h include/const.h include/protect.h include/string.h include/proc.h \
			include/proto.h include/global.h include/fs_const.h include/fs.h include/hd.h include/fs_misc.h \
			include/fat32.h
	$(CC) $(CFLAGS) -o $@ $<

# assist.o	added by mingxuan 2019-5-17
kernel/assist.o: kernel/assist.c include/type.h include/const.h include/protect.h include/string.h include/proc.h \
			include/proto.h include/global.h include/fs_const.h include/fs.h include/hd.h include/fs_misc.h \
			include/fat32.h
	$(CC) $(CFLAGS) -o $@ $<

# vfs.o	added by mingxuan 2019-5-17
kernel/vfs.o: kernel/vfs.c
	$(CC) $(CFLAGS) -o $@ $<

# printf.o	added by mingxuan 2019-5-19
lib/printf.o: lib/printf.c
	$(CC) $(CFLAGS) -o $@ $<

# vsprintf.o	added by mingxuan 2019-5-19	
lib/vsprintf.o: lib/vsprintf.c
	$(CC) $(CFLAGS) -o $@ $<

# scanf.o	added by mingxuan 2019-5-19
lib/scanf.o: lib/scanf.c
	$(CC) $(CFLAGS) -o $@ $<

# keyboard.o	added by mingxuan 2019-5-19
kernel/kerboard.o: kernel/keyboard.c
	$(CC) $(CFLAGS) -o $@ $<

# console.o	added by mingxuan 2019-5-19
kernel/console.o: kernel/console.c
	$(CC) $(CFLAGS) -o $@ $<

# tty.o	added by mingxuan 2019-5-19
kernel/tty.o: kernel/tty.c
	$(CC) $(CFLAGS) -o $@ $<

# shell.o	added by mingxuan 2019-5-19
kernel/shell.o: kernel/shell.c
	$(CC) $(CFLAGS) -o $@ $<

kernel/list.o: kernel/list.c
	$(CC) $(CFLAGS) -o $@ $<

include ./init/makefile	 #added by mingxuan 2019-5-19
# include ./user/makefile	 #added by mingxuan 2019-5-21
