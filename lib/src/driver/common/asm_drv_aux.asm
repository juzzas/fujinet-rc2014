EXTERN asm_buffer_init
EXTERN asm_buffer_tx_char
EXTERN asm_buffer_tx_flush

PUBLIC asm_drv_aux_init
PUBLIC asm_drv_aux_put_char
PUBLIC asm_drv_aux_flush

SECTION code_user

DEFC MODEM_DEV_ID = 0x50

asm_drv_aux_init:
    push ix

    ld ix, ctx_aux_tx
    ld a, (ctx_aux_device_id)
    ld de, ctx_aux_tx_buffer
    call asm_buffer_init

    ld ix, ctx_aux_rx
    ld a, (ctx_aux_device_id)
    ld de, ctx_aux_rx_buffer
    call asm_buffer_init

    pop ix
    ret


; entry:
;   C = character
asm_drv_aux_put_char:
    push ix
    ld ix, ctx_aux_tx
    call asm_buffer_tx_char
    pop ix
    ret

asm_drv_aux_flush:
    push ix
    push hl
    ld ix, ctx_aux_tx
    call asm_buffer_tx_flush
    pop hl
    pop ix
    ret

SECTION data_user

ctx_aux_tx:
    DEFB 0, 0, 0, 0

ctx_aux_rx:
    DEFB 0, 0, 0, 0

ctx_aux_tx_buffer:
    DEFS 64

ctx_aux_rx_buffer:
    DEFS 64

ctx_aux_device_id:
    DEFB MODEM_DEV_ID