
DEFC BDOS = 0x0005

SECTION code_user

PUBLIC _console_init, _console_tx, _console_rx, _console_rx_avail, _console_puts

;;Initialised the FujiNet hardware

_console_init:
    ret


;; Output byte from Console device
; entry:
;    L = byte
_console_tx:
    push bc
    ld c, 2     ; CP/M 3 C_WRITE (Returns A=byte)
    ld a, l
    call BDOS
    pop bc
    ret


;; Input byte from Console device
; exit:
;    L = byte
_console_rx:
    push bc
    ld c, 1    ; CP/M C_READ (Returns A=byte)
    call BDOS
    ld l, a
    pop bc
    ret


;; Number of bytes available in receive buffer
; exit:
;    L = number of bytes in buffer
_console_rx_avail:
    push bc
    ld c, 11     ; CP/M 3 C_STATUS (Returns A=0 or 0FFh)
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


_console_puts:
    ld a, (hl)
    or a
    ret z

    push bc
    push de
    push hl
    ld c, 2     ; CP/M 3 C_WRITE (Returns A=byte)
    ld e, a
    call BDOS
    pop hl
    pop de
    pop bc
    inc hl
    jr _console_puts

