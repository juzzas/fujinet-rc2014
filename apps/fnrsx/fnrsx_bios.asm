
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
DEFC cpm_bdos_jumpblock_size = 99  ; for CP/M 3 -- https://www.seasip.info/Cpm/bios.html

EXTERN patched_home
EXTERN patched_seldsk
EXTERN patched_settrk
EXTERN patched_setsec
EXTERN patched_setdma
EXTERN patched_read
EXTERN patched_write
EXTERN patched_sectran

PUBLIC jumpblock_copy
PUBLIC patch_bios

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

;	ld bc,cpm_home
;	ld de,patched_home
;	call patch

;	ld bc,cpm_seldsk
;	ld de,patched_seldsk
;	call patch

;	ld bc,cpm_settrk
;	ld de,patched_settrk
;	call patch

;	ld bc,cpm_setsec
;	ld de,patched_setsec
;	call patch

;	ld bc,cpm_setdma
;	ld de,patched_setdma
;	call patch

;	ld bc,cpm_read
;	ld de,patched_read
;	call patch
;
;	ld bc,cpm_write
;	ld de,patched_write
;	call patch
;
;	ld bc,cpm_sectran
;	ld de,patched_sectran
;	call patch
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
;;BOOT (function 0)
;;
;;This function is completely implementation-dependent and should never be called from user code.

patched_boot:
	push hl
	push af
	;ld hl,log_boot
	;call display_message
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

;;------------------------------------------------------------------------

jumpblock_copy:
	defs cpm_bdos_jumpblock_size


