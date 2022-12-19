; compile with  z88dk-z80asm -v -b -reloc-info -l -s -m -g -omain.bin fnrsx_main.asm

EXTERN bdos_handler
EXTERN old_bdos

driver_bdos:
    jp bdos_handler
    jp driver_init


driver_init:
    ;; install our bdos, thereby reducing tpa.
    ld hl, 0x0005
    ld de, old_bdos
    ld bc, 3
    ldir

    ;; install our bdos
    ld a, 0xc3
    ld hl, driver_bdos
    ld (0x0005), a
    ld (0x0006), hl

