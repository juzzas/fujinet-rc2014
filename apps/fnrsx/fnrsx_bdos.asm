; compile with  z88dk-z80asm -v -b -reloc-info -l -s -m -g -omain.bin fnrsx_main.asm

PUBLIC bdos_handler
PUBLIC old_bdos

old_bdos:
    DEFS 3


bdos_handler:
    jp old_bdos

