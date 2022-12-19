; compile with  z88dk-z80asm -v -b -reloc-info -l -s -m -g -omain.bin fnrsx_main.asm

jumptable:
    jp REL_init
    jp REL_test


REL_init:
    ld hl, 0x1234
    ret

REL_test:
    call REL_subroutine
    ret

REL_subroutine:
    ret
