EXTERN asm_buffer_init
EXTERN asm_buffer_tx_char
EXTERN asm_buffer_tx_flush
EXTERN asm_buffer_rx
EXTERN asm_buffer_rx_avail

PUBLIC asm_drv_aux_init
PUBLIC asm_drv_aux_tx
PUBLIC asm_drv_aux_tx_flush
PUBLIC asm_drv_aux_rx
PUBLIC asm_drv_aux_rx_avail

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
asm_drv_aux_tx:
    push ix
    ld ix, ctx_aux_tx
    call asm_buffer_tx_char
    call asm_buffer_tx_flush
    pop ix
    ret

asm_drv_aux_tx_flush:
    push ix
    push hl
    ld ix, ctx_aux_tx
    call asm_buffer_tx_flush
    pop hl
    pop ix
    ret

asm_drv_aux_rx:
    push ix
    push hl
    ld ix, ctx_aux_rx
    call asm_buffer_rx
    pop hl
    pop ix
    ret

asm_drv_aux_rx_avail:
    push ix
    push hl
    ld ix, ctx_aux_rx
    call asm_buffer_rx_avail
    pop hl
    pop ix
    ret

SECTION data_user

ctx_aux_tx:
    DEFB 0, 0, 0, 0, 0

ctx_aux_rx:
    DEFB 0, 0, 0, 0, 0

ctx_aux_tx_buffer:
    DEFS 64

ctx_aux_rx_buffer:
    DEFS 64

ctx_aux_device_id:
    DEFB MODEM_DEV_ID