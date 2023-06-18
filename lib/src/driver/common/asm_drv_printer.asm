EXTERN asm_buffer_init
EXTERN asm_buffer_tx_char
EXTERN asm_buffer_tx_flush

PUBLIC asm_drv_printer_init
PUBLIC asm_drv_printer_put_char
PUBLIC asm_drv_printer_flush
PUBLIC asm_drv_printer_status

SECTION code_user

DEFC PRINTER_DEV_ID = 0x41

asm_drv_printer_init:
    ; TODO - do status call
    push ix
    ld ix, ctx_printer
    ld a, (ctx_printer_device_id)
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
    push hl
    ld ix, ctx_printer
    call asm_buffer_tx_flush
    pop hl
    pop ix
    ret

asm_drv_printer_status:
    ; TODO - check the actual status from the last buffer send
    ld a, 0
    ret

SECTION data_user

ctx_printer:
    DEFB 0, 0, 0, 0

ctx_printer_buffer:
    DEFS 64

ctx_printer_device_id:
    DEFB PRINTER_DEV_ID