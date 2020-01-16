org 07c00h
LABEL_START:	
	mov	ax, cs
	mov	ds, ax
	mov	es, ax
	mov	ss, ax
	mov	sp, 7c00h

	; 清屏
	mov	ax, 0600h		; AH = 6,  AL = 0h
	mov	bx, 0700h		; 黑底白字(BL = 07h)
	mov	cx, 0			; 左上角: (0, 0)
	mov	dx, 0184fh		; 右下角: (80, 50)
	int	10h			; int 10h

	mov	dh, 0			; 
	call	DispStr			; "Finding active partition..."
		
	mov	bx, 0
	mov	dh, 1
CHECK_PARTITION:
	mov	dl, [7c00h+446+bx]
	mov	ax, word [7c00h+446+bx+8]
	
	push	dx
	push	bx	
	cmp	dl, 80h
	jz	LABLE_ACTIVE
DisplayPInfo:	
	call	DispStr
	pop 	bx
	pop	dx
	cmp	dh, 4
	jz	LABLE_END
	inc	dh
	add	bx, 16
	jmp	CHECK_PARTITION

LABLE_ACTIVE:
	mov	[SecEntry], ax
	add	dh, 4
	jmp	DisplayPInfo
	
LABLE_END:
	;mov	ah,0h
	;int	16h

	mov	ax, [SecEntry]
	mov 	cl, 1
	mov	bx, 1000h
	mov 	es, bx
	mov 	bx, 7c00h
	call 	ReadSector
	jmp 	1000h:7c00h

SecEntry:	dw 0



;============================================================================
;字符串
;----------------------------------------------------------------------------
; 为简化代码, 下面每个字符串的长度均为 MessageLength
MessageLength		equ	27
BootMessage:		db	"Finding active partition..."	; 27字节, 不够则用空格补齐. 序号 0

Message1		db	"    partition 1: not active"	;
Message2		db	"    partition 2: not active"	;
Message3		db	"    partition 3: not active"	;
Message4		db	"    partition 4: not active"	;

Message5		db	"    partition 1: active    "	;
Message6		db	"    partition 2: active    "	;
Message7		db	"    partition 3: active    "	;
Message8		db	"    partition 4: active    "	;




;============================================================================

;----------------------------------------------------------------------------
; 函数名: DispStr
;----------------------------------------------------------------------------
; 作用:
;	显示一个字符串, 函数开始时 dh 中应该是字符串序号(0-based)
DispStr:
	push	ax
	push	dx
	mov	ax, MessageLength
	mul	dh
	add	ax, BootMessage
	mov	bp, ax			; ┓
	mov	ax, ds			; ┣ ES:BP = 串地址
	mov	es, ax			; ┛
	mov	cx, MessageLength	; CX = 串长度
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 0007h		; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov	dl, 0
	int	10h			; int 10h
	pop	dx
	pop 	ax
	ret


;----------------------------------------------------------------------------
; 函数名: ReadSector
;----------------------------------------------------------------------------
; 作用:
;	从第 ax 个 Sector 开始, 将 cl 个 Sector 读入 es:bx 中

DAPS:   DB 0x10               ; size of packet 
        DB 0                  ; Always 0
        D_CL	DW 1          ; number of sectors to transfer
	D_BX	DW 7C00h      ; transfer buffer (16 bit segment:16 bit offset) 
	D_ES	DW 0000h	
	LBA_Lo	DD 1	      ; lower 32-bits of 48-bit starting LBA
	LBA_Hi	DD 0	      ; upper 32-bits of 48-bit starting LBAs

ReadSector:
	
	mov	[D_CL],   cl
	mov	[D_BX],   bx
	mov	[D_ES],   es
	mov	[LBA_Lo], ax
	mov	dl, 0x80		; 驱动器号 (0 表示 A 盘)

.GoOnReading:
	mov	ah, 42h			; 读
	mov 	si, DAPS
	int	13h
	jc	.GoOnReading		; 如果读取错误 CF 会被置为 1, 这时就不停地读, 直到正确为止

	ret


times 444 -($-$$) db 0
dw 0x0000

