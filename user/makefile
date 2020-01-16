# INIT = init.bin
INIT = shell_0.bin shell_1.bin shell_2.bin \
	   execTest.bin pthreadTest.bin forkTest.bin \
	   yieldTest.bin sleepTest.bin

# you can add your elf in $(BIN) latter
BIN = $(INIT)
TAR_NAME = app.tar
HD = ../b.img

ASM			= nasm
ASMFLAGS	= -I ../include/ -f elf
AR 			= ar
ARFLAGS		= rcs
DASM		= objdump
CC			= gcc
LD			= ld
CFLAGS		= -I ../include/ -m32 -c -fno-builtin -fno-stack-protector -Wall -Wextra -g
LDFLAGS		= -m elf_i386 -s
DASMFLAGS	= -D
OBJSULIB 	= ../lib/ulib.a
START_O 	= initstart.o

# INSTALL_INIT / INSTALL_TAR
INSTALL_TYPE = INSTALL_TAR
# INSTALL_TYPE = INSTALL_INIT

# oranges文件系统在硬盘上的起始扇区
PART_START_SECTOR = 92049	
# 写入硬盘的起始位置
INSTALL_PHY_SECTOR = 93000
# assert(INSTALL_PHY_SECTOR > PART_START_SECTOR)
# 写入硬盘的文件大小
INSTALL_NR_SECTORS = 1000

INSTALL_START_SECTOR = $(shell echo $$(($(INSTALL_PHY_SECTOR)-$(PART_START_SECTOR))))
SUPER_BLOCK_ADDR = $(shell echo $$((($(PART_START_SECTOR)+1)*512)))

ifeq ($(INSTALL_TYPE),INSTALL_INIT)
	INSTALL_FILENAME:=$(INIT)
else ifeq ($(INSTALL_TYPE),INSTALL_TAR)
	INSTALL_FILENAME:=$(TAR_NAME)
endif

# All Phony Targets
.PHONY : everything final clean_user realclean_user disasm all install $(OBJSULIB)

# Default starting position
everything : $(BIN)

all : realclean_user everything

final : all clean

clean_user :
	rm -f *.o $(BIN)

realclean_user :
	rm -f $(BIN) *.o *.tar

install : all 
	@#将super block->magic 置0
	dd if=/dev/zero of=$(HD) bs=1 conv=notrunc seek=$(SUPER_BLOCK_ADDR) count=1
ifeq ($(INSTALL_TYPE),INSTALL_INIT)
else ifeq ($(INSTALL_TYPE),INSTALL_TAR)
	tar vcf $(TAR_NAME) $(BIN)
else
	@echo Error: unknown INSTALL_TYPE $(INSTALL_TYPE) && exit 1
endif
	@echo INSTALL_FILENAME=$(INSTALL_FILENAME) 
	dd if=$(INSTALL_FILENAME) of=$(HD) bs=512 conv=notrunc seek=$(INSTALL_PHY_SECTOR) count=$(INSTALL_NR_SECTORS) 

# clean_init:
#	rm -rf $(TAR_NAME) $(BIN) *.map *.bin

$(START_O) : initstart.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

# added by mingxuan 2019-5-22
shell_0.o: shell_0.c ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

shell_0.bin : shell_0.o $(START_O) $(OBJSULIB)
	$(LD) $(LDFLAGS) -o $@ $^

shell_1.o: shell_1.c ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

shell_1.bin : shell_1.o $(START_O) $(OBJSULIB)
	$(LD) $(LDFLAGS) -o $@ $^

shell_2.o: shell_2.c ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

shell_2.bin : shell_2.o $(START_O) $(OBJSULIB)
	$(LD) $(LDFLAGS) -o $@ $^


execTest.o: execTest.c ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

execTest.bin : execTest.o $(START_O) $(OBJSULIB)
	$(LD) $(LDFLAGS) -o $@ $^

pthreadTest.o: pthreadTest.c ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

pthreadTest.bin : pthreadTest.o $(START_O) $(OBJSULIB)
	$(LD) $(LDFLAGS) -o $@ $^

forkTest.o: forkTest.c ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

forkTest.bin : forkTest.o $(START_O) $(OBJSULIB)
	$(LD) $(LDFLAGS) -o $@ $^

yieldTest.o: yieldTest.c ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

yieldTest.bin : yieldTest.o $(START_O) $(OBJSULIB)
	$(LD) $(LDFLAGS) -o $@ $^

sleepTest.o: sleepTest.c ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

sleepTest.bin : sleepTest.o $(START_O) $(OBJSULIB)
	$(LD) $(LDFLAGS) -o $@ $^
