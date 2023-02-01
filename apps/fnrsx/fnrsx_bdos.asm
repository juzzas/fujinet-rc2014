; compile with  z88dk-z80asm -v -b -reloc-info -l -s -m -g -omain.bin fnrsx_main.asm

PUBLIC bdos_handler
PUBLIC old_bdos
extern fujinet_dcb_exec

old_bdos:
    DEFS 3


bdos_handler:
    ld a, c
    cp 0xf0
    jr z, handle_bdos_fujinet
    jp old_bdos

; entry: de = fuginet_dcb
; exit:   a = FUJINET_RC
handle_bdos_fujinet:
    push hl
    ld hl, de
    call fujinet_dcb_exec
    pop hl
    ret




