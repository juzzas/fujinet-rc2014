;
; Created by jskists on 12/10/2022.
;

EXTERN fujinet_hal_init
EXTERN fujinet_hal_tx
EXTERN fujinet_hal_rx
EXTERN fujinet_hal_rx_avail
EXTERN fujinet_hal_rx_avail_timeout
EXTERN fujinet_hal_assert_cmd
EXTERN fujinet_hal_wait_cmd_ready
EXTERN fujinet_hal_wait_cmd_ready_timeout
EXTERN fujinet_hal_assert_spi_cs
EXTERN fujinet_hal_deassert_cmd
EXTERN fujinet_hal_deassert_spi_cs

PUBLIC fujinet_init
PUBLIC fujinet_dcb_exec


DEFC DCB_TX_BUFFER_L = 4
DEFC DCB_TX_BUFFER_H = 5
DEFC DCB_TX_BUFFER_LEN_L = 6
DEFC DCB_TX_BUFFER_LEN_H = 7

DEFC DCB_RX_BUFFER_L = 8
DEFC DCB_RX_BUFFER_H = 9
DEFC DCB_RX_BUFFER_LEN_L = 10
DEFC DCB_RX_BUFFER_LEN_H = 11

DEFC DCB_TIMEOUT_L = 12
DEFC DCB_TIMEOUT_H = 13

DEFC FUJINET_RC_OK = 0
DEFC FUJINET_RC_NOT_IMPLEMENTED = 1
DEFC FUJINET_RC_NOT_SUPPORTED = 2
DEFC FUJINET_RC_INVALID = 3
DEFC FUJINET_RC_TIMEOUT = 4
DEFC FUJINET_RC_NO_ACK = 5
DEFC FUJINET_RC_NO_COMPLETE = 6


SECTION code_user

;void fujinet_init(void) {
;    fujinet_hal_init();
;}
fujinet_init:
    jp fujinet_hal_init




;uint8_t fujinet_checksum(uint8_t *buf, uint16_t len)
;{
;    uint8_t checksum = 0x00;
;
;    for (uint16_t i = 0; i < len; i++)
;        checksum ^= buf[i];
;
;    return checksum;
;}

; HL = pointer to buffer
; BC = length of buffer
;
; uses: E = current checksum
;
; exit: A = checksum
fujinet_checksum:
    ld e, 0

fc_loop:
    ; update checksum
    ld a, e
    xor (hl)
    ld e, a

    ; increment pointer
    inc hl

    ; decrement counter
    dec bc
    ld a, b
    or c
    jr nz, fc_loop

    ld a, e
    ret

; -----------------------------------------------------------------------------

;FUJINET_RC fujinet_dcb_exec(struct fujinet_dcb *dcb) {

; ENTRY: HL = dcb
; USES:  IX = pointer to dcb
;        BC = counters
;        DE = working register
; EXIT:   A = FUJINET_RC

fujinet_dcb_exec:

;    FUJINET_RC rc = FUJINET_RC_OK;
;    command_frame frame;
    push hl
    pop ix
    ld de, dcb_frame
    ldi
    ldi
    ldi
    ldi
    push ix
    pop hl
    ld bc, 4
    push de
    call fujinet_checksum
    pop de
    ld (de), a

;
;    fujinet_hal_assert_cmd(true);
    call fujinet_hal_assert_cmd

; Wait for CMD_RDY to be asserted
    ld d, (ix+DCB_TIMEOUT_H)
    ld e, (ix+DCB_TIMEOUT_L)
    call fujinet_hal_wait_cmd_ready_timeout
    jp c, exec_exit_TIMEOUT

    ; transmit command frame
    ld hl, dcb_frame
    ld bc, 5
    call tx_buff
;
;    fujinet_hal_assert_cmd(false);


;
;    if (!is_rx_avail(dcb->timeout)) {
;        rc = FUJINET_RC_TIMEOUT;
;        goto err_exit;
;    }

;    uint8_t ack = fujinet_hal_rx();
    call fujinet_hal_wait_cmd_ready
    call fujinet_hal_assert_spi_cs
    call fujinet_hal_rx
    ex af, af'
    call fujinet_hal_deassert_spi_cs
    ex af, af'


;    if (ack != 'A') {
;        rc = FUJINET_RC_NO_ACK;
;        goto err_exit;
;    }
    cp 'A'
    jp nz, exec_exit_NO_ACK


;    if (dcb->buffer_bytes != 0) {
    ld b, (ix+DCB_TX_BUFFER_LEN_H)
    ld c, (ix+DCB_TX_BUFFER_LEN_L)
    ld a, b
    or c
    jr z, exec_skip_tx_buf

;        for (uint16_t i = 0; i < dcb->buffer_bytes; i++)
;            fujinet_hal_tx(dcb->buffer[i]);
    call fujinet_hal_wait_cmd_ready
    ld h, (ix+DCB_TX_BUFFER_H)
    ld l, (ix+DCB_TX_BUFFER_L)

;    push bc
;    push hl

    call tx_buff

;        fujinet_hal_tx(fujinet_checksum(dcb->buffer, dcb->buffer_bytes));
;    pop hl
;    pop bc
;    call fujinet_checksum
;    call fujinet_hal_tx

;        if (!is_rx_avail(dcb->timeout)) {
;            rc = FUJINET_RC_TIMEOUT;
;            goto err_exit;
;        }

;        ack = fujinet_hal_rx();
    call fujinet_hal_wait_cmd_ready
    call fujinet_hal_assert_spi_cs
    call fujinet_hal_rx
    ex af, af'
    call fujinet_hal_deassert_spi_cs
    ex af, af'

;        if (ack != 'A') {
;            rc = FUJINET_RC_NO_ACK;
;            goto err_exit;
;        }
    cp 'A'
    jp nz, exec_exit_NO_ACK

;    }

exec_skip_tx_buf:
;        if (!is_rx_avail(dcb->timeout)) {
;            rc = FUJINET_RC_TIMEOUT;
;            goto err_exit;
;        }
;
;    if (dcb->response_bytes != 0) {
    ld b, (ix+DCB_RX_BUFFER_LEN_H)
    ld c, (ix+DCB_RX_BUFFER_LEN_L)
    ld a, b
    or c
    jr z, exec_skip_rx_buf

    call fujinet_hal_wait_cmd_ready

;        for (uint16_t i = 0; i < dcb->response_bytes; i++)
;            dcb->response[i] = fujinet_hal_rx();
    ld h, (ix+DCB_RX_BUFFER_H)
    ld l, (ix+DCB_RX_BUFFER_L)
    call rx_buff

;        uint8_t chk = fujinet_hal_rx();
;    call fujinet_hal_rx
;    ex af, af'
;    call fujinet_hal_deassert_spi_cs
;    ex af, af'
    ; TODO - check checksum


exec_skip_rx_buf:
;    if (!is_rx_avail(dcb->timeout)) {
;        rc = FUJINET_RC_TIMEOUT;
;        goto err_exit;
;    }
    call fujinet_hal_wait_cmd_ready
    call fujinet_hal_assert_spi_cs

;    ack = fujinet_hal_rx();
    call fujinet_hal_rx
    ex af, af'
    call fujinet_hal_deassert_spi_cs
    ex af, af'

;    if (ack != 'C') {
;        rc = FUJINET_RC_NO_COMPLETE;
;        goto err_exit;
;    }
    cp 'C'
    jp nz, exec_exit_NO_COMPLETE

;    return rc;
    call fujinet_hal_deassert_cmd
    ld a, FUJINET_RC_OK
    ret

exec_exit_TIMEOUT:
    call fujinet_hal_deassert_cmd
    ld a, FUJINET_RC_TIMEOUT
    ret

exec_exit_NO_COMPLETE:
    call fujinet_hal_deassert_cmd
    ld a, FUJINET_RC_NO_COMPLETE
    ret

exec_exit_NO_ACK:
    call fujinet_hal_deassert_cmd
    ld a, FUJINET_RC_NO_ACK
    ret

; -----------------------------------------------------------------------------

dcb_frame:
    defb 0, 0, 0, 0
dcb_frame_checksum:
    defb 0xa5

; -----------------------------------------------------------------------------

;ENTRY HL = pointer to buffer
;      BC = bytes to send
tx_buff:
    ld a, c  ; is BC >= 64
    and 0xc0
    or b
    jr z, tx_buff_64 ; BC is < 64

    push bc
    ld bc, 64
    call tx_buff_64

    ex de, hl ; save pointer to buffer
    pop hl    ; subtract 64 from count
    ld bc, 64
    sbc hl, bc
    ld b, h
    ld c, l
    ex de, hl
    call fujinet_hal_wait_cmd_ready

    ld a, c  ; is BC 0?
    or b
    jr nz, tx_buff
    ret


; tranmit a transaction of 64 or less bytes
;ENTRY HL = pointer to buffer
;      BC = bytes to send
tx_buff_64:
    call fujinet_hal_assert_spi_cs
tx_buff1:
    ; BC > 0?
    ld a, b
    or c
    jr z, tx_buff2

    ; tx byte at pointer
    ld a, (hl)
    call fujinet_hal_tx

    ; loop
    inc hl
    dec bc
    jr tx_buff1

tx_buff2:
    call fujinet_hal_deassert_spi_cs
    ret

; -----------------------------------------------------------------------------

;ENTRY HL = pointer to buffer
;      DE = timeout (TODO)
;      BC = bytes to receive
rx_buff:
    ld a, c  ; is BC 0?
    or b
    ret z

    ld a, c  ; is BC >= 64
    and 0xc0
    or b
    jr z, rx_buff_64 ; BC is < 64

    push bc
    ld bc, 64
    call rx_buff_64

    ex de, hl ; save pointer to buffer
    pop hl    ; subtract 64 from count
    ld bc, 64
    sbc hl, bc
    ld b, h
    ld c, l
    ex de, hl
    call fujinet_hal_wait_cmd_ready

    ld a, c  ; is BC 0?
    or b
    jr nz, rx_buff
    ret


; tranmit a transaction of 64 or less bytes
;ENTRY HL = pointer to buffer
;      DE = timeout (TODO)
;      BC = bytes to receive
rx_buff_64:
    call fujinet_hal_assert_spi_cs
rx_buff1:
    ; BC > 0?
    ld a, b
    or c
    jr z,rx_buff2

    ; rx byte to pointer
    call fujinet_hal_rx
    ld (hl), a

    ; loop
    inc hl
    dec bc
    jr rx_buff1

rx_buff2:
    call fujinet_hal_deassert_spi_cs
    ret

