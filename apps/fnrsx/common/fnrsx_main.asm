; Copyright 2023 Justin Skists
;
; Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
; documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
; rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
; persons to whom the Software is furnished to do so, subject to the following conditions:
;
; The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
; Software.
;
; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
; WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
; COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
; OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

DEFC FUJINET_FN_DCB = 0xf0
DEFC FUJINET_FN_POLL = 0xf1
DEFC FUJINET_FN_CHAIN = 0xf2
DEFC L_WRITE = 0x05
DEFC BDOS = 5

EXTERN fujinet_init
EXTERN fujinet_dcb_exec
EXTERN fujinet_poll_proceed
EXTERN init_patch_bdos
EXTERN patch_bios
EXTERN chain_load

EXTERN asm_drv_printer_init
EXTERN asm_drv_printer_flush
EXTERN asm_drv_modem_flush
EXTERN handle_bdos_l_write

ORG 0x100

serial:   defb 0,0,0,0,0,0
start:    jp bdos_handler  ; start of program
next:     defb 0xc3        ; jump instruction to
          defw 0           ; next module in line
prev:     defw 0           ; previous module
remove:   defb 0           ; remove flag (0xff = remove)
nonbank:  defb 0           ; nonbank flag
name:     defm "FNRSX   "  ; any 8-character name
loader:   defb 0           ; loader flag
          defb 0,0         ; reserved area


bdos_handler:
    ; initialise RSX
    ld a,(flag_initialised)
    or a
    call z, do_init

    ; override check
    ld a, c

    cp FUJINET_FN_DCB
    jp z, handle_bdos_fujinet_dcb

    cp FUJINET_FN_POLL
    jp z, handle_bdos_fujinet_poll_proceed

    ;cp FUJINET_FN_CHAIN
    ;jp z, chain_load

    cp L_WRITE
    jp z, handle_bdos_l_write

    ; else, just flush any pending buffers
    push af
    push bc
    push de
    push hl
    push ix
    call asm_drv_printer_flush
    ;call asm_drv_modem_flush
    pop ix
    pop hl
    pop de
    pop bc
    pop af

    jp next

;;------------------------------------------------------------------------

do_init:
    push de
    push bc
    push hl
    push ix
    call fujinet_init

    call init_patch_bdos
    call patch_bios

    call asm_drv_printer_init
    ;call asm_drv_modem_init

    ld hl, log_boot
    call display_message

    ld a, 0xff  ; mark as initialised
    ld (flag_initialised), a
    pop ix
    pop hl
    pop bc
    pop de
    ret

;;------------------------------------------------------------------------

; entry: de = address of FujiNet DCB command
; uses: ix, bc, hl, a, a'
; exit: a = return code (FUJINET_RC)
handle_bdos_fujinet_dcb:
    ld a, d ; de is zero, just initialise?
    or e
    ret z

    ld hl, de
    call fujinet_dcb_exec
    ret

flag_initialised:
    defb 0


;;------------------------------------------------------------------------

; entry: none
; uses: ix, bc, hl, a, a'
; exit: a = 1: asserted, 0: not asserted
handle_bdos_fujinet_poll_proceed:
    call fujinet_poll_proceed
    ret

;;------------------------------------------------------------------------

display_char:
    push hl
    ld c,2			;; console output function id
    ld e,a			;; ASCII character
    call next		;; call BDOS to execute function
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
    defb "FujiNet RSX initialised", 13, 10, "$"
