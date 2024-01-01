; Fujinet interface for CamelForth

; ---------------------------------------------------------------------------
; configuration parameters; can be changed at runtime

org 0xf800

; jump table
EXTERN fujinet_init
EXTERN fujinet_dcb_exec
EXTERN fujinet_poll_proceed

; xx00 FUJINET_INIT
JT_FUJINET_INIT:
    jp fujinet_init

; xx03 FUJINET_DCB
JT_FUJINET_DCB:
    jp do_FUJINET_DCB

; xx06 FUJINET_POLL
JT_FUJINET_POLL:
    jp do_FUJINET_POLL

; ---------------------------------------------------------------------------

; xx03 FUJINET_DCB
do_FUJINET_DCB:
    rst 0x28
    ld hl, bc
    call fujinet_dcb_exec
    ld c, a
    ld b, 0
    ret

; xx06 FUJINET_POLL
do_FUJINET_POLL:
    call fujinet_poll_proceed
    or a

    jr z, noproceed
    ld bc, -1
    rst 0x20
    ret

noproceed:
    ld bc, 0
    rst 0x20
    ret

