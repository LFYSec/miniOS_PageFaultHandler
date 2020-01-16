INIT = init.bin
# INIT = shell_0.bin shell_1.bin shell_2.bin 

# you can add your elf in $(BIN) latter
BIN = $(INIT)
TAR_NAME = app.tar
HD = ../b.img
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


default:
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

clean_init:
	rm -rf $(TAR_NAME) $(BIN) *.map *.bin