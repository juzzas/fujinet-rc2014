EXTERN asm_buffer_init
EXTERN asm_buffer_tx_char
EXTERN asm_buffer_tx_flush

PUBLIC asm_drv_printer_init
PUBLIC asm_drv_printer_put_char
PUBLIC asm_drv_printer_flush

SECTION code_user

DEFC PRINTER_DEV_ID = 0x41

asm_drv_printer_init:
    push ix
    ld ix, ctx_printer
    ld a, PRINTER_DEV_ID
    ld de, ctx_printer_buffer
    call asm_buffer_init
    pop ix
    ret


; entry:
;   C = character
asm_drv_printer_put_char:
    push ix
    ld ix, ctx_printer
    call asm_buffer_tx_char
    pop ix
    ret

asm_drv_printer_flush:
    push ix
    ld ix, ctx_printer
    call asm_buffer_tx_flush
    pop ix
    ret

SECTION data_user

ctx_printer:
    DEFB 0, 0, 0, 0

ctx_printer_buffer:
    DEFS 64

