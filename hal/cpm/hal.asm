; RC2014 PIO address 0x68 to 0x6B (using module SC103 Z80 PIO)
;   0x68 = Port A data
;   0x69 = Port B data
;   0x6A = Port A control
;   0x6B = Port B control

DEFC PIO_DATA_A = 0x68
DEFC PIO_DATA_B = 0x69
DEFC PIO_CTRL_A = 0x6a
DEFC PIO_CTRL_B = 0x6b

DEFC BDOS = 0x0005

SECTION code_user

PUBLIC _fujinet_hal_init, _fujinet_hal_tx, _fujinet_hal_rx, _fujinet_hal_rx_avail, _fujinet_hal_assert_cmd

;;Initialised the FujiNet hardware
;void fujinet_hal_init(void);

_fujinet_hal_init:
            LD   A, 0b11001111
            OUT  (PIO_CTRL_A), A  ;Port A = PIO 'control' mode
            LD   A, 0b00000000
            OUT  (PIO_CTRL_A),A   ;Port A = all lines are outputs

            LD   A, 0b11111111
            ld (assert_val), a
            OUT  (PIO_DATA_A),A   ;Port A = all lines are "active low"

            ret


;;
;void fujinet_hal_tx(uint8_t ch);
; entry:
;    L = byte
_fujinet_hal_tx:
    push bc
    ld c, 4     ; CP/M 3 A_READ (Returns A=byte)
    ld a, l
    call BDOS
    pop bc
    ret


;; Input byte from FujiNet device
; exit:
;    L = byte
_fujinet_hal_rx:
    push bc
    ld c, 3    ; CP/M A_READ (Returns A=byte)
    call BDOS
    ld l, a
    pop bc
    ret


;; Number of bytes available in receive buffer
; exit:
;    L = number of bytes in buffer
_fujinet_hal_rx_avail:
    push bc
    ld c, 7     ; CP/M 3 A_STATIN (Returns A=0 or 0FFh)
    call BDOS
    or a
    jr nz, rx_avail_yes

rx_avail_no:
    ld l, 0
    pop bc
    ret

rx_avail_yes:
    ld l, 1   ; let's just say that there is only one byte.
    pop bc
    ret


;; Assert the COMMAND line
; entry:
;    L = assert
_fujinet_hal_assert_cmd:
    ld a, l
    cp 0
    jr nz, do_assert

    ; de-assert
    ld a, (assert_val)
    or 0x01
    ld (assert_val), a
    out (PIO_DATA_A), a
    ret

do_assert:
    ld a, (assert_val)
    and 0xfe
    ld (assert_val), a
    out (PIO_DATA_A), a
    ret



SECTION data_user

assert_val:
    DEFB 0