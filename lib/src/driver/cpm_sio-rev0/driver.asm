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

DEFC BDOS_A_READ = 3
DEFC BDOS_A_WRITE = 4
DEFC BDOS_A_STATIN = 7

SECTION code_user

PUBLIC fujinet_hal_init
PUBLIC fujinet_hal_tx
PUBLIC fujinet_hal_rx
PUBLIC fujinet_hal_rx_avail
PUBLIC fujinet_hal_rx_avail_timeout
PUBLIC fujinet_hal_assert_cmd

;EXTERN msleep
EXTERN asm_z80_delay_ms

;;Initialised the FujiNet hardware
;void fujinet_hal_init(void);

fujinet_hal_init:
    ld a, 0b11001111
    out (PIO_CTRL_A), a   ;Port A = PIO 'control' mode
    ld a, 0b00000000
    out (PIO_CTRL_A),a    ;Port A = all lines are outputs

    ld a, 0b11111111
    ld (assert_val), a
    out (PIO_DATA_A), a   ;Port A = all lines are "active low"

    ret


;;
;void fujinet_hal_tx(uint8_t ch);
; entry:
;    A = byte
fujinet_hal_tx:
    push hl
    push bc
    push de
    ld c, BDOS_A_WRITE     ; CP/M 3 A_WRITE (Returns A=byte)
    ld e, a
    push ix
    push iy
    call BDOS
    pop iy
    pop ix
    pop de
    pop bc
    pop hl
    ret


;; Input byte from FujiNet device
; exit:
;    A = byte
fujinet_hal_rx:
    push hl
    push bc
    ld c, BDOS_A_READ    ; CP/M A_READ (Returns A=byte)
    push de
    push ix
    push iy
    call BDOS ; (A = rx byte)
    pop iy
    pop ix
    pop de
    pop bc
    pop hl
    ret


; ENTRY: DE = timeout in 10ths of seconds
; EXIT:   A = bytes in buffer

fujinet_hal_rx_avail_timeout:
    call fujinet_hal_rx_avail
    cp 0
    ret nz

    ;timer timed out?
    ld a, d
    or e
    ret z ; (A = 0, here)

    ld hl, 100
    push bc
    push de
    ;call msleep
    call asm_z80_delay_ms
    pop de
    pop bc
    dec de
    jr fujinet_hal_rx_avail_timeout


;; Number of bytes available in receive buffer
; exit:
;    A = number of bytes in buffer
fujinet_hal_rx_avail:
    push hl
    push bc
    ld c, BDOS_A_STATIN     ; CP/M 3 A_STATIN (Returns A=0 or 0FFh)
    push de
    push ix
    push iy
    call BDOS
    pop iy
    pop ix
    pop de
    or a
    jr nz, rx_avail_yes

rx_avail_no:
    ld a, 0
    pop bc
    pop hl
    ret

rx_avail_yes:
    ld a, 1   ; let's just say that there is only one byte.
    pop bc
    pop hl
    ret


;; Assert the COMMAND line
; entry:
;    A = assert
fujinet_hal_assert_cmd:
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