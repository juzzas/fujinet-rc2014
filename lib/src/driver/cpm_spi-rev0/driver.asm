; RC2014 PIO address 0x68 to 0x6B (using module SC103 Z80 PIO)
;   0x68 = Port A data
;   0x69 = Port B data
;   0x6A = Port A control
;   0x6B = Port B control

DEFC PIO_DATA_A = 0x68
DEFC PIO_DATA_B = 0x69
DEFC PIO_CTRL_A = 0x6a
DEFC PIO_CTRL_B = 0x6b

DEFC MASK_CMD_RDY = 0b10000000
DEFC MASK_PROCEED = 0b01000000
DEFC MASK_MOSI = 0b00000010
DEFC MASK_MISO = 0b00000001

DEFC SPI_MASK = 0b11111100

DEFC BIT_CMD_RDY = 7
DEFC BIT_CMD = 7

SECTION code_user

PUBLIC fujinet_hal_init
PUBLIC fujinet_hal_tx
PUBLIC fujinet_hal_rx
PUBLIC fujinet_hal_assert_cmd
PUBLIC fujinet_hal_deassert_cmd
PUBLIC fujinet_hal_assert_spi_cs
PUBLIC fujinet_hal_deassert_spi_cs
PUBLIC fujinet_hal_wait_cmd_ready
PUBLIC fujinet_hal_wait_cmd_ready_timeout
PUBLIC fujinet_poll_proceed

;EXTERN msleep
EXTERN asm_z80_delay_ms

;;Initialised the FujiNet hardware
;void fujinet_hal_init(void);

fujinet_hal_init:
    ld a, 0b11001111
    out (PIO_CTRL_A), a   ;Port A = PIO 'control' mode
    ld a, 0b00000000
    out (PIO_CTRL_A),a    ;Port A = all lines are outputs

    ld a, 0b11111110
    ld (assert_val), a
    out (PIO_DATA_A), a   ;Port A = all lines are "active low"
                          ;         with idle SPI clock


    ld a, 0b11001111
    out (PIO_CTRL_B), a   ;Port B = PIO 'control' mode
    ld a, 0b11111111
    out (PIO_CTRL_B),a    ;Port B = all lines are inputs

    ret


;;
;void fujinet_hal_tx(uint8_t ch);
; entry:
;    A = byte
; uses:
;    A, BC
fujinet_hal_tx:
    push bc

    ld b, 8
    ld c, a
tx_bit:
    sla c ; bit to send in C-flag

    jr c, tx_high

tx_low:
    ld a, (assert_val)
    and SPI_MASK ; clock low, low MOSI
    out (PIO_DATA_A), a
    inc a  ; clock high, low MOSI
    out (PIO_DATA_A), a
    dec a  ; clock low, low MOSI
    out (PIO_DATA_A), a
    djnz tx_bit

    ;ld (assert_val), a
    pop bc
    ret

tx_high:
    ld a, (assert_val)
    or 2  ; clock low, high MOSI
    out (PIO_DATA_A), a
    inc a ; clock high, high MOSI
    out (PIO_DATA_A), a
    dec a  ; clock low, high MOSI
    out (PIO_DATA_A), a
    djnz tx_bit

    ;ld (assert_val), a
    pop bc
    ret


;; Input byte from FujiNet device
; exit:
;    A = byte
; uses:
;    A, BC
fujinet_hal_rx:
    push bc

    ld b, 8
    ld c, 0

rx_bit:
    sla c  ; shift before fetching next bit
           ; doesn't matter the first time in the loop

    ld a, (assert_val)
    or 3  ; clock high, high MOSI

    out (PIO_DATA_A), a

    in a, (PIO_DATA_B)
    and MASK_MISO

    jr z, miso_not_high ; if zero, skip setting bit
    ld a, c
    or 1
    ld c, a

miso_not_high:
    ld a, (assert_val)
    and SPI_MASK
    or 2  ; clock low, high MOSI
    out (PIO_DATA_A), a

    djnz rx_bit

    ;ld (assert_val), a
    ld a, c
    pop bc
    ret


;; Assert the COMMAND line
; entry:
;    A = assert
fujinet_hal_assert_cmd:
    ; de-assert
    ld a, (assert_val)
    and 0x7f
    ld (assert_val), a
    out (PIO_DATA_A), a
    ret

fujinet_hal_deassert_cmd:
    ld a, (assert_val)
    or 0x80
    ld (assert_val), a
    out (PIO_DATA_A), a
    ret

;; Assert the SPI CS line
; entry:
;    A = assert
fujinet_hal_assert_spi_cs:
    ld a, (assert_val)
    and 0b11111011
    ld (assert_val), a
    out (PIO_DATA_A), a
    ret

fujinet_hal_deassert_spi_cs:
    ; de-assert
    ld a, (assert_val)
    or 0x04
    ld (assert_val), a
    out (PIO_DATA_A), a
    ret


;; Poll the CCOMMAND_READY line
;  exit: A = 0, then asserted
fujinet_hal_poll_cmd_ready:
    in a, (PIO_DATA_B)
    and MASK_CMD_RDY
    ret

fujinet_hal_wait_cmd_ready:
    in a, (PIO_DATA_B)
    and MASK_CMD_RDY
    jr nz, fujinet_hal_wait_cmd_ready
    ret

; ENTRY: DE = timeout in 10ths of seconds
; EXIT:   C-flag if timeout
fujinet_hal_wait_cmd_ready_timeout:
    in a, (PIO_DATA_B)
    and MASK_CMD_RDY    ; zero is asserted
    ret z

    ;timer timed out?
    ld a, d
    or e
    scf
    ret z ; (A = 1, here)

    ld hl, 1
    push bc
    push de
    ;call msleep
    call asm_z80_delay_ms
    pop de
    pop bc
    dec de
    jr fujinet_hal_wait_cmd_ready_timeout

SECTION data_user
assert_val:
    DEFB 0


SECTION code_user

fujinet_poll_proceed:
    in a, (PIO_DATA_B)
    and MASK_PROCEED
    jr z, proceed_asserted

    ld a, 0
    ret

proceed_asserted:
    ld a, 1
    ret
