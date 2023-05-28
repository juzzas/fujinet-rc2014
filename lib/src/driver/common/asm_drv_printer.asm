EXTERN asm_buffer_init
EXTERN asm_buffer_tx_char
EXTERN asm_buffer_tx_flush

PUBLIC asm_drv_printer_init
PUBLIC asm_drv_printer_put_char
PUBLIC asm_drv_printer_flush

SECTION code_user

DEFC PRINTER_DEV_ID = 0x40

asm_drv_printer_init:
    ld ix, ctx_printer
    ld a, PRINTER_DEV_ID
    ld de, ctx_printer_buffer
    jp asm_buffer_init


; entry:
;   E = character
asm_drv_printer_put_char:
    ld ix, ctx_printer
    jp asm_buffer_tx_char

asm_drv_printer_flush:
    ld ix, ctx_printer
    jp asm_buffer_tx_flush

SECTION data_user

ctx_printer:
    DEFS 4

ctx_printer_buffer:
    DEFS 64

