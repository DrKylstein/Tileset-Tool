			.486
			OPTION		SEGMENT:USE16
			DOSSEG
			
			INCLUDE		<./../include/keen1.inc>
CSEG		SEGMENT		PARA COMMON USE16 'CODE'


			INCLUDE		<./../include/k1cseg.asm>
			ASSUME		ds:dseg, cs:cseg, ss:sseg

th_tile_count	equ	<ds:[8290h+0]>
th_frame_count	equ	<ds:[8290h+2]>	
th_gfx_size		equ	<es:[8290h+4]>	
th_info_loc_lo	equ	<ds:[8290h+6]>
th_info_loc_hi	equ	<ds:[8290h+8]>	
th_anim_loc_lo	equ	<ds:[8290h+10]>
th_anim_loc_hi	equ	<ds:[8290h+12]>	
th_gfx_loc_lo	equ	<ds:[8290h+14]>	
th_gfx_loc_hi	equ	<ds:[8290h+16]>	
th_pal_loc_lo	equ	<ds:[8290h+18]>	
th_pal_loc_hi	equ	<ds:[8290h+20]>	


tileinfo_loc		=	3A1Ch

tileanim_loc1	=	0A8h	
tileanim_loc2	=	8372h
tileanim_loc3	=	9740h

palette_loc	=	0E800h

start:
; startup code
	ORG	13BFh
	jmp	main_c
		
; SwapTiles called from load_level_data; overwriting mov current_level, ax instruction
; On entry, ax contains level number
SwapTiles	proc	near
	MapPal:
		mov	ax, 4200h	; move file pointer to start of palette
		mov	dx, th_pal_loc_lo
		mov	cx, th_pal_loc_hi
		int	21h

		mov	ax, 3F00h			;read palette from file (bx contains handle)
		mov	cx, 768			;num bytes to copy

		mov	dx, palette_loc	;ds:dx = read destination
		int	21h		
		
		jmp	CloseFile

	ORG 1400h
	
		push	si
		push	di
	
		mov	current_level, ax	; overwrote this	

; Save graphics registers
SaveGfxReg:
		mov	dx, 3C4h	; save selector for sequencer
		in	al, dx
		xor	ah, ah
		push	ax
	
		mov	al, 2
		out	dx, al
	
		inc	dx	; save sequencer, register 2 (map mask register)
		in	al, dx
		push	ax

; Get  asciiz filename of tileset.  Stored at offset 16*current_level in TILEHEAD.CK1 (ds:52A4)

GetName:	
	
		mov	ax, 3D00h	; open file
		mov	dx, offset aTILEHEAD_CK1	; ds:dx points to file
		int	21h
		mov	bx, ax	; save file handle
		
		mov	ax, 4200h	; move pointer to start of filename
		xor	cx, cx
		mov	dx, current_level	; cx:dx = distance = current_level * 16
		shl	dx, 4
		int	21h
	
		mov	ax, 3F00h	; read from file (bx = filehandle)
		mov	cx, 10h	; filenames are 16 bytes
		mov	dx, offset string_buf	; ds:dx = destination buffer
		int	21h
		
		mov	ah, 3Eh	;close file (handle in bx)
		int	21h	
; Name in string_buf.  Open datafile

OpenTileSheet:
	
		mov	ax, 3D00h	; open file
		mov	dx,  offset string_buf ; asciiz filename
		int	21h
		mov	bx, ax	; save file handle

; Read tilesheet header into string_buf
; header starts at first byte, thus file pointer is set properly from open instruction

	mov	ax, 3F00h	; read from file (bx = filehandle)
	mov	cx, 40h	; fileheader is x bytes
	mov	dx, offset string_buf	; ds:dx = destination buffer
	int	21h

; treat string buffer like a tilesheet header STRUCT

MapTileInfo:	
	mov	ax, 4200h	; move file pointer to start of tileinfo
	mov	dx, th_info_loc_lo
	mov	cx, th_info_loc_hi
	int	21h

	mov	ax, 3F00h			;read tileinfo from file (bx contains handle)
	mov	cx, 910 * 5			;num bytes to copy
							; (5 bytes per tile)
	mov	dx, tileinfo_loc	;ds:dx = read destination
	int	21h	

MapTileAnim:
	mov	ax, 4200h	; move file pointer to start of tileanim (frames 0-3)
	mov	dx, th_anim_loc_lo
	mov	cx, th_anim_loc_hi
	int	21h

    mov si, 910 * 16		; length of tileanim for 8 frames  = 16x tilecount
		
	mov	ax, 3F00h	;read frames 0-3 from file (bx contains handle)
	mov	cx, si ;num bytes to copy (tileanim_len/2)	
	shr cx, 1  ;
	mov	dx, tileanim_loc1	;ds:dx = read destination
	int	21h	
	
	cmp word ptr th_frame_count, 4
	je MapTileGfx
	
	mov	ax, 3F00h	; read frames 4,5 from file
	mov	cx, si ; (tileanim_len/4)
	shr cx, 2
	mov	dx, tileanim_loc2
	int	21h

	mov	ax, 3F00h	; read frames 6,7 from file
	mov	cx, si ; (tileanim_len/4)
	shr cx, 2
	mov	dx, tileanim_loc3
	int	21h

MapTileGfx:	
	mov		ax, 4200h	; move filepointer to graphics
	mov		dx, th_gfx_loc_lo
	mov		cx, th_gfx_loc_hi
	int		21h

	mov		ax, 5	; set write mode = 00
	mov		dx, 3CEh
	out		dx, ax
	
	push	es	; save es
	
	push	ds	
	pop		es	; es = ds

	push	ds	; save ds
	xor		cx, cx	; register count = 0
	lds		si, tile_gfx	; ds:si = tile graphics fptr (video mem)

	
ReadGfxPlane:	
	mov		ah, 1	; 
	shl		ah, cl	; ah --> write to plane i
	mov		al, 2	; al --> select map mask register
	mov		dx, 3C4h	; (sequencer registers)
	out		dx, ax
		
	push	cx	; save count
	
		
	mov		dx, si 	; ds:dx = far ptr to tile graphics
	mov		cx,th_gfx_size	;One plane of tile data
	mov		ah, 3Fh
	int		21h	;read into video memory

	
	pop		cx	; restore count
	inc		cx
	cmp		cl, 4
	jl		ReadGfxPlane	; repeat for all four planes
	
	pop		ds	;restore ds, es
	pop		es
	
	jmp	MapPal
	
CloseFile:	mov	ah, 3Eh	; close file (handle in bx)
	int	21h	

RestoreGfxReg:	mov	al, 2	; restore registers	
	mov	dx, 3C4h
	out	dx, al
	
	pop	ax
	inc	dx
	out	dx, al
	
	pop	ax
	dec	dx
	out	dx, al
	
	pop	di
	pop	si
	ret

SwapTiles 	ENDP
;===========================================
; Pogo and jump fixes
;===========================================
	ORG 1500h

pogofix:	
	mov		word ptr temp_sprite.think, offset think_keen_pogo_ground
	mov		word ptr temp_sprite.time, 0
	mov		ax, temp_sprite.velX
	mov		temp_sprite.varB, ax
	mov		temp_sprite.velX, word ptr 0
	jmp		pogoahead

jumpfix:	
	mov		word ptr temp_sprite.think, offset think_keen_ground
	mov		ax, 7
	push	ax
	call	set_cur_sound
	inc	sp
	inc	sp
	
	jmp	jumpdone

; pogo fix
	ORG	3FACh
	test	si, 2
	jz		pogoahead
	cmp		word ptr temp_sprite.velY, 0
	jl		pogoahead
	jmp		pogofix

	ORG	3FCAh
pogoahead:

; jump fix
	ORG	3D68h
	test	si, 2
	jz		jumpahead
	cmp		word ptr temp_sprite.velY, 0
	jl		jumpahead
	jmp		jumpfix
	
	ORG	3D80h
jumpahead:

	ORG	3E01h
jumpdone:


	ORG	1557h
main_c:


	ORG 676h
call SwapTiles	


cseg	ENDS

dseg	SEGMENT	PARA USE16
		include <./../include/k1dseg.asm>
		include <./../include/v2inc.asm>
dseg	ENDS

sseg	SEGMENT	PARA USE16 STACK
sseg	ENDS


	END	start
