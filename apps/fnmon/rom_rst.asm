PUBLIC _z80_rst_38h

EXTERN handle_acia_int

SECTION code_user

_z80_rst_38h:
    di
    call handle_acia_int
    ei
    reti