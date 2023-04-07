
DEFC bios = 0
DEFC bdos = 5
DEFC drive_user = 4

DEFC cpm_boot = 0
DEFC cpm_wboot = 0+3
DEFC cpm_home = 21+3
DEFC cpm_seldsk = 24+3
DEFC cpm_settrk = 27+3
DEFC cpm_setsec = 30+3
DEFC cpm_setdma = 33+3
DEFC cpm_read = 36+3
DEFC cpm_write = 39+3
DEFC cpm_sectran = 45+3

DEFC first_drive = 12
DEFC last_drive = 15

DEFC cpm_sector_size = 128
DEFC cpm_bdos_jumpblock_size = 0x33



;;-----------------------------------------------------------------------------------------------

patch_bios:
	; Now copy jumpblock and patch it for our use
	ld hl,(1)	;; this is the address of the JP for wboot
	or a
	ld de,3
	sbc hl,de
	;; HL = JP for boot
	push hl
	ld de,jumpblock_copy
	ld bc,cpm_bdos_jumpblock_size
	ldir
	pop hl

	;; now patch
	;;ld bc,cpm_boot
	;;ld de,patched_boot
	;;call patch

	;;ld bc,cpm_wboot
	;;ld de,patched_wboot
	;;call patch

	ld bc,cpm_home
	ld de,patched_home
	call patch

	ld bc,cpm_seldsk
	ld de,patched_seldsk
	call patch

	ld bc,cpm_settrk
	ld de,patched_settrk
	call patch

	ld bc,cpm_setsec
	ld de,patched_setsec
	call patch

	ld bc,cpm_setdma
	ld de,patched_setdma
	call patch

	ld bc,cpm_read
	ld de,patched_read
	call patch

	ld bc,cpm_write
	ld de,patched_write
	call patch

	ld bc,cpm_sectran
	ld de,patched_sectran
	call patch
	ret

;;------------------------------------------------------------------------------------------------------------
patch:
	push hl
	add hl,bc
	ld a,0xc3
	ld (hl),a
	inc hl
	ld (hl),e
	inc hl
	ld (hl),d
	pop hl
	ret


;;------------------------------------------------------------------------------------------------------------

;; From seasip.info:
;;SETTRK (function 10)
;;
;;Set the track in BC - 0 based.

patched_settrk:
	call is_current_drive_ours
	jp c,jumpblock_copy+cpm_settrk
	ld (track),bc
	ret

;;------------------------------------------------------------------------------------------------------------

;; From seasip.info:
;;SETSEC (function 11)
;;
;;Set the sector in BC. Under CP/M 1 and 2 a sector is 128 bytes. Under CP/M 3 the sector size is given in the Disk Parameter Block.

patched_setsec:
	call is_current_drive_ours
	jp c,jumpblock_copy+cpm_setsec
	ld (sec),bc
	ret

;;------------------------------------------------------------------------------------------------------------
;; From seasip.info:
;;SETDMA (function 12)
;;
;;The next disc operation will read its data from (or write its data to) the address given in BC.

patched_setdma:
	call is_current_drive_ours
	jp c,jumpblock_copy+cpm_setdma
	ld (dma),bc
	ret

;;------------------------------------------------------------------------------------------------------------
;; From seasip.info:
;;BOOT (function 0)
;;
;;This function is completely implementation-dependent and should never be called from user code.

patched_boot:
	push hl
	push af
	ld hl,log_boot
	call display_message
	pop af
	pop hl
	jp jumpblock_copy+cpm_boot


;;------------------------------------------------------------------------------------------------------------
;; From seasip.info:
;;WBOOT (function 1)
;;
;;Reloads the command processor and (on some systems) the BDOS as well. How it does this is implementation-dependent; it may use the reserved tracks of a floppy disc or extra memory.
;; Probably only needed for CP/M 2.2
patched_wboot:
	;; do normal wboot
	call jumpblock_copy+cpm_wboot
	;; repatch jump block
	;call patch_bdos
	call patch_bios
	ret


;;------------------------------------------------------------------------------------------------------------
;; From seasip.info:
;; SELDSK
;; Select the disc drive in register C (0=A:, 1=B: ...). Called with E=0 or 0FFFFh.
patched_seldsk:
	;; is it one of our drives?
	ld a,c
	cp first_drive
	jp c,jumpblock_copy+cpm_seldsk
	cp last_drive+1
	jp nc,jumpblock_copy+cpm_seldsk
	;; yes it is, return the dph for our drives
	ld a,c
	sub first_drive
	add a,a
	add a, dphlist & 255
	ld l,a
	ld a,dphlist/256
	adc a,0
	ld h,a
	ld a,(hl)
	inc hl
	ld h,(hl)
	ld l,a		;; return DPH
	ret

;;----------------------------------------------------------------------------------------------

is_current_drive_ours:
	ld a,(drive_user) ;; read current drive/user
	and 0xf
	cp first_drive
	ret c
	cp last_drive+1	;; if nc then it's greater and not our drive
	;; if c it is our drive
	ccf
	ret

;;----------------------------------------------------------------------------------------------
;; Each CPM2.2 sector is 128 bytes

;;----------------------------------------------------------------------------------------------
;;READ (function 13)

;;Read the currently set track and sector at the current DMA address. Returns A=0 for OK, 1 for unrecoverable error, 0FFh if media changed.

patched_read:
	call is_current_drive_ours
	jp c,jumpblock_copy+cpm_read

	;; fake read
	ld hl,(dma)
	ld bc,cpm_sector_size
read1:
	ld (hl),0xe5
	inc hl
	dec bc
	ld a,b
	or c
	jr nz,read1
	xor a
	ret

;;-----------------------------------------------------------------------------------------------------------
;;
;; From seasip.info:
;;SECTRAN (function 16)
;;
;;Translate sector numbers to take account of skewing.
;;
;;On entry, BC=logical sector number (zero based) and DE=address of translation table.
;;On exit, HL contains physical sector number. On a system with hardware skewing, this would normally ignore DE and return either BC or BC+1.
patched_sectran:
	call is_current_drive_ours
	jp c,jumpblock_copy+cpm_sectran
	ld l,c
	ld h,b
	ret


;;-----------------------------------------------------------------------------------------------------------
;; seasip.info:
;;WRITE (function 14)
;;
;;Write the currently set track and sector. C contains a deblocking code:
;;
patched_write:
	call is_current_drive_ours
	jp c,jumpblock_copy+cpm_write
	;; it's our drive

	;; fake write
	xor a
	ret


;;-----------------------------------------------------------------------------------------------------------
;; seasip.info:
;;WRITE (function 14)
;;
;;Write the currently set track and sector. C contains a deblocking code:
;;
patched_home:
	call is_current_drive_ours
	jp c,jumpblock_copy+cpm_home
	;; it's our drive

	;; set track 0
	ld bc,0
	ld (track),a
	ret


jumpblock_copy:
	defs cpm_bdos_jumpblock_size

;;-----------------------------------------------------------------------------------------------
dphlist:
	defw dph_fndisk1
	defw dph_fndisk2
	defw dph_fndisk3
	defw dph_fndisk4

;;-----------------------------------------------------------------------------------------------

dph_fndisk1:
	defw 0	;; address of xlt
	defw 0,0,0	;; used as workspace for cpm
	defw dirbuf		;; dirbuf
	defw dpb_fndisk1	;; dpb
	defw 0		;; csv
	defw drv_fndisk1_alv		;; alv



dph_fndisk2:
	defw 0	;; address of xlt
	defw 0,0,0	;; used as workspace for cpm
	defw dirbuf		;; dirbuf
	defw dpb_fndisk2	;; dpb
	defw 0		;; csv
	defw drv_fndisk2_alv		;; alv


dph_fndisk3:
	defw 0	;; address of xlt
	defw 0,0,0	;; used as workspace for cpm
	defw dirbuf		;; dirbuf
	defw dpb_fndisk3	;; dpb
	defw 0		;; csv
	defw drv_fndisk3_alv		;; alv


dph_fndisk4:
	defw 0	;; address of xlt
	defw 0,0,0	;; used as workspace for cpm
	defw dirbuf		;; dirbuf
	defw dpb_fndisk4	;; dph
	defw 0		;; csv
	defw drv_fndisk4_alv		;; alv

drv_fndisk1_alv:
	;;defs 1024
drv_fndisk2_alv:
	;;defs 1024
drv_fndisk3_alv:
	;;defs 1024
drv_fndisk4_alv:
	defs 1024

dpb_fndisk1:
	defw 10	;; spt
	defb 3	;; bsh
	defb 7	;; blm
	defb 0 	;; exm
	defw 100	;; dsm
	defw 64	;; drm
	defb 0xc0 ;; al0
	defb 0 	;; al1
	defw 0	;; cks
	defw 0	;; off


dpb_fndisk2:
	defw 10	;; spt
	defb 3	;; bsh
	defb 7	;; blm
	defb 0 	;; exm
	defw 100	;; dsm
	defw 64	;; drm
	defb 0xc0 ;; al0
	defb 0 	;; al1
	defw 0	;; cks
	defw 100	;; off


dpb_fndisk3:
	defw 10	;; spt
	defb 3	;; bsh
	defb 7	;; blm
	defb 0 	;; exm
	defw 100	;; dsm
	defw 64	;; drm
	defb 0xc0 ;; al0
	defb 0 	;; al1
	defw 0	;; cks
	defw 200	;; off


dpb_fndisk4:
	defw 10	;; spt
	defb 3	;; bsh
	defb 7	;; blm
	defb 0 	;; exm
	defw 100	;; dsm
	defw 64	;; drm
	defb 0xc0 ;; al0
	defb 0 	;; al1
	defw 0	;; cks
	defw 400	;; off

dirbuf:
	defs 128

track:
	defw 0
sec:
	defw 0
dma:
	defw 0

;;------------------------------------------------------------------------

 display_char:
 push hl
 ld c,2			;; console output function id
 ld e,a			;; ASCII character
 call bdos		;; call BDOS to execute function
 pop hl
 ret

 ;;------------------------------------------------------------------------

 ;; HL = pointer to null terminated message
 display_message:
 ld a,(hl)				;; get ASCII character
 inc hl					;; increment pointer for next character
 cp '$'					;; end of message marker
 ret z					;; quit if end of message marker found.

 call display_char		;; send character to console output
 jp display_message		;; loop for next char

 log_boot:
 defb "Seen boot$"