;==============================================================================================================================
BaseOfStack		equ	0x07c00		; Boot状态下堆栈基地址
DATA_BUF_OFF 		equ  	0x2000		; 目录被加载的缓冲区地址
STACK_ADDR  		equ  	0x7bea 		; 堆栈栈顶
OSLOADER_SEG 		equ  	0x09000 	; 起始段地址     
OSLOADER_SEG_OFF	equ		0x0100

FAT_START_SECTOR 	equ  	0x820	  	; FAT表的起始扇区号  DWORD
DATA_START_SECTOR 	equ  	0xd6a	  	; 数据区起始扇区号  DWORD
DIR_PER_SECTOR  	equ  	0x10  		; 每个扇区所容纳的目录 BYTE

; 扩展磁盘服务所使用的地址包
DAP_SECTOR_HIGH  	equ  	4  		; 起始扇区号的高32位 ( 每次调用需要重置 ) DWORD
DAP_SECTOR_LOW  	equ  	8  		; 起始扇区号的低32位 ( 每次调用需要重置 ) DWORD
DAP_BUFFER_SEG  	equ  	10  		; 缓冲区段地址   ( 每次调用需要重置 ) WORD
DAP_BUFFER_OFF  	equ  	12  		; 缓冲区偏移   ( 每次调用需要重置 ) WORD  
DAP_RESERVED2  		equ  	13  		; 保留字节
DAP_READ_SECTORS 	equ  	14  		; 要处理的扇区数(1 - 127 )
DAP_RESERVED1  		equ  	15  		; 保留字节
DAP_PACKET_SIZE  	equ  	16  		; 包的大小为16字节

CURRENT_CLUSTER  	equ  	20  		; 当前正在处理的簇号 DWORD

; 目录项结构
OFF_START_CLUSTER_HIGH  equ  	20  		; 起始簇号高位  WORD
OFF_START_CLUSTER_LOW  	equ  	26  		; 起始簇号低位  WORD

; 相关常量
DIR_NAME_FREE    	equ  	0x00  		; 该项是空闲的
DIR_ENTRY_SIZE    	equ  	32  		; 每个目录项的尺寸

; 簇属性
CLUSTER_MASK    	equ  	0FFFFFFFH 	; 簇号掩码
CLUSTER_LAST    	equ  	0FFFFFF8H   	;0xFFFFFFF8-0xFFFFFFFF表示文件的最后一个簇
;==============================================================================================================================
	org	07c00h

	jmp	START
	nop

	BS_OEM		DB	'mkfs.fat'	;文件系统标志
	BPB_BytesPerSec	DW	0x200		;每扇区字节数
	BPB_SecPerClu 	DB	1		;每簇扇区数
	BPB_RsvdSecCnt	DW	0x20		;保留扇区数
	BPB_NumFATs	DB	2		;FAT表数
	BPB_RootEntCnt	DW 	0		;FAT32不使用
	BPB_TotSec16	DW	0		;扇区总数
	BPB_Media	DB 	0xf8		;介质描述符
	BPB_FATSz16	DW	0		;每个FAT表的大小扇区数
	BPB_SecPerTrk	DW	0x3f		;每磁道扇区数
	BPB_NumHeads	DW	0xff		;磁头数
	BPB_HiddSec	DD	0		;分区已使用扇区数
	BPB_TotSec32	DD	0x015791	;文件系统大小扇区数
	BS_SecPerFAT	DD	0x02a5		;每个FAT表大小扇区数
	BS_Flag		DW	0		;标记
	BS_Version	DW	0		;版本号
	BS_RootClus	DD	2		;根目录簇号
	BS_FsInfoSec 	DW	1		;FSINFO扇区号
	BS_BackBootSec 	DW	6		;备份引导扇区位置
	BS_Unuse1	DD	0		;未使用
	BS_Unuse2	DD	0		;未使用
	BS_Unuse3	DD 	0		;未使用
	BS_DriveNum	DB	0x80		;设备号
	BS_Unuse4	DB	0x01		;未使用
	BS_ExtBootFlag	DB	0x29		;扩展引导标志
	BS_VolNum	DD	0xbe3a8ff5	;卷序列号
	BS_VolName	DB	'MZY hd boot'	;卷标

START:	
	cld
	xor     ax, ax
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, STACK_ADDR
	mov  	bp, BaseOfStack

	mov 	dword [bp - DAP_SECTOR_HIGH ],	00h
	mov 	byte  [bp - DAP_RESERVED1   ], 	00h
	mov 	byte  [bp - DAP_RESERVED2   ], 	00h
	mov 	byte  [bp - DAP_PACKET_SIZE ], 	10h
	mov	byte  [bp - DAP_READ_SECTORS],  01h
	mov	byte  [bp - DAP_BUFFER_SEG  ],	00h

	jmp	_SEARCH_LOADER

_MISSING_LOADER:     ; 显示没有装载程序
_DISK_ERROR:      ; 显示磁盘错误信息   
	JMP  $

ReadSector:
	pusha
	mov	ah, 42h
	lea 	si, [BP - DAP_PACKET_SIZE]
	mov	dl, [BS_DriveNum]
	int 	13h
	jc 	_DISK_ERROR
	popa
	ret

_SEARCH_LOADER:
	mov	word [bp - DAP_BUFFER_OFF], DATA_BUF_OFF
	mov	eax, dword [BS_RootClus]
	mov	dword [bp - CURRENT_CLUSTER], eax

_NEXT_ROOT_CLUSTER:
	dec 	eax
	dec 	eax
	xor	ebx, ebx
	mov	bl, byte [BPB_SecPerClu]
	mul	ebx
	add	eax, DATA_START_SECTOR
	mov	dword [BP - DAP_SECTOR_LOW], eax
	mov	dl, [BPB_SecPerClu]

_NEXT_ROOT_SECTOR:
	call	ReadSector

	mov	di, DATA_BUF_OFF
	mov	bl, DIR_PER_SECTOR

_NEXT_ROOT_ENTRY:
	cmp	byte [di], DIR_NAME_FREE
	jz	_MISSING_LOADER

	push 	di;
	mov	si, LoaderName
	mov	cx, 10
	repe	cmpsb
	jcxz	_FOUND_LOADER

	pop	di
	add	di, DIR_ENTRY_SIZE
	dec	bl
	jnz	_NEXT_ROOT_ENTRY

	dec	dl
	jz 	_CHECK_NEXT_ROOT_CLUSTER
	inc	dword [bp - DAP_SECTOR_LOW]
	jmp 	_NEXT_ROOT_SECTOR

_CHECK_NEXT_ROOT_CLUSTER:

	 ; 计算FAT所在的簇号和偏移 
	 ; FatOffset = ClusterNum*4
	 XOR  EDX,EDX
	 MOV  EAX,DWORD[BP - CURRENT_CLUSTER]
	 SHL  EAX,2
	 XOR  ECX,ECX
	 MOV  CX,WORD [ BPB_BytesPerSec ]
	 DIV  ECX  ; EAX = Sector EDX = OFFSET
	 ADD  EAX, FAT_START_SECTOR
	 MOV  DWORD [ BP - DAP_SECTOR_LOW ], EAX 
	   
	 call  ReadSector
	  
	 ; 检查下一个簇
	 MOV  DI,DX
	 ADD  DI,DATA_BUF_OFF
	 MOV  EAX,DWORD[DI]  ; EAX = 下一个要读的簇号
	 AND  EAX,CLUSTER_MASK
	 MOV  DWORD[ BP - CURRENT_CLUSTER ],EAX
	 CMP  EAX,CLUSTER_LAST  ; CX >= 0FFFFFF8H，则意味着没有更多的簇了
	 JB  _NEXT_ROOT_CLUSTER
	 JMP  _MISSING_LOADER

_FOUND_LOADER:
	 ; 目录结构地址放在DI中
	 pop  di
	 xor  eax, eax
	 mov  ax, [di + OFF_START_CLUSTER_HIGH] ; 起始簇号高32位
	 shl  ax, 16
	 mov  ax, [di + OFF_START_CLUSTER_LOW]  ; 起始簇号低32位
	 mov  dword [ bp - CURRENT_CLUSTER ], eax
	 mov  cx, OSLOADER_SEG      ; CX  = 缓冲区段地址 

_NEXT_DATA_CLUSTER:
	 ; 根据簇号计算扇区号
	 DEC  EAX
	 DEC  EAX  
	 XOR  EBX,EBX 
	 MOV  BL, BYTE [ BPB_SecPerClu ]
	 MUL  EBX 
	 ADD  EAX, DATA_START_SECTOR
	 MOV  DWORD[ BP - DAP_SECTOR_LOW  ], EAX
	 MOV  BL , BYTE [BPB_SecPerClu]

	 ; 设置缓冲区
	 MOV  WORD [ BP - DAP_BUFFER_SEG   ], CX
	 MOV  WORD [ BP - DAP_BUFFER_OFF   ], OSLOADER_SEG_OFF

_NEXT_DATA_SECTOR:
	 ; 读取簇中的每个扇区(内层循环)
	 ; 注意 : 通过检查文件大小，可以避免读取最后一个不满簇的所有大小
	 call  ReadSector
	 
	 ; 更新地址，继续读取
	 MOV  AX, WORD [BPB_BytesPerSec]
	 ADD  WORD  [BP - DAP_BUFFER_OFF], ax 
	 INC  DWORD [BP - DAP_SECTOR_LOW]  ; 递增扇区号
	 DEC  BL        ; 内层循环计数
	 JNZ  _NEXT_DATA_SECTOR
	  
	 ; 更新读取下一个簇的缓冲区地址
	 MOV  CL, BYTE [ BPB_SecPerClu ]
	 MOV  AX, WORD [BPB_BytesPerSec]
	 SHR  AX, 4
	 MUL  CL
	 ADD  AX, WORD [ BP - DAP_BUFFER_SEG ] 
	 MOV  CX, AX ; 保存下一个簇的缓冲区段地址
	 
	 ;====================================================================
	 ; 检查是否还有下一个簇(读取FAT表的相关信息)
	 ;  LET   N = 数据簇号
	 ;  THUS FAT_BYTES  = N*4  (FAT32)
	 ;  FAT_SECTOR = FAT_BYTES / BPB_BytesPerSec
	 ;  FAT_OFFSET = FAT_BYTES % BPB_BytesPerSec
	 ;====================================================================
	 
	 ; 计算FAT所在的簇号和偏移 
	 MOV  EAX,DWORD [BP - CURRENT_CLUSTER]
	 XOR  EDX,EDX
	 SHL  EAX,2
	 XOR  EBX,EBX
	 MOV  BX,WORD [ BPB_BytesPerSec ]
	 DIV  EBX   ; EAX = Sector  EDX = Offset
	 
	 ; 设置缓冲区地址
	 ADD  EAX, FAT_START_SECTOR
	 MOV  DWORD [ BP - DAP_SECTOR_LOW ], EAX 
	 MOV  WORD [BP - DAP_BUFFER_SEG  ], 00H 
	 MOV  WORD [BP - DAP_BUFFER_OFF  ], DATA_BUF_OFF

	 ; 读取扇区
	 CALL  ReadSector
	  
	 ; 检查下一个簇
	 MOV  DI,DX
	 ADD  DI,DATA_BUF_OFF
	 MOV  EAX,DWORD[DI]  ; EAX = 下一个要读的簇号
	 AND  EAX,CLUSTER_MASK
	 MOV  DWORD[ BP - CURRENT_CLUSTER ],EAX
	 CMP  EAX,CLUSTER_LAST  ; CX >= 0FFFFFF8H，则意味着没有更多的簇了
	 JB  _NEXT_DATA_CLUSTER

_RUN_LOADER: 
	 mov  dl, [BS_DriveNum]
	 jmp  OSLOADER_SEG:OSLOADER_SEG_OFF

LoaderName     db "LOADER  BIN"       ; 第二阶段启动程序 FDOSLDR.BIN

times 	510-($-$$)	db	0	; 填充剩下的空间，使生成的二进制代码恰好为512字节
dw 	0xaa55				; 结束标志
