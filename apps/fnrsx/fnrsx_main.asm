; compile with  z88dk-z80asm -v -b -reloc-info -l -s -m -g -omain.bin fnrsx_main.asm

EXTERN bdos_handler
EXTERN old_bdos

EXTERN fujinet_init

jump_table:
    jp driver_init
    jp bdos_handler


driver_init:
    ;; install our bdos, thereby reducing tpa.
    ld hl, 0x0005
    ld de, old_bdos
    ld bc, 3
    ldir

    ;; install our bdos
    ld a, 0xc3
    ld hl, jump_table + 3
    ld (0x0005), a
    ld (0x0006), hl

    CALL fujinet_init

    ret

