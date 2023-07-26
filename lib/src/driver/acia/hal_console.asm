
DEFC BDOS = 0x0005

SECTION code_user

PUBLIC _console_init, _console_tx, _console_rx, _console_rx_avail, _console_puts
EXTERN acia_putc, acia_getc, acia_pollc

;;Initialised the FujiNet hardware

_console_init:
    ret


;; Output byte from Console device
; entry:
;    L = byte
_console_tx:
    ld a, l
    rst 0x08
    ret


;; Input byte from Console device
; exit:
;    L = byte
_console_rx:
    rst 0x10
    ld h, 0
    ld l, a
    ret


;; Number of bytes available in receive buffer
; exit:
;    L = number of bytes in buffer
_console_rx_avail:
    rst 0x18
    ld h, 0
    ld l, a
    ret


_console_puts:
    ld a, (hl)
    or a
    ret z

    push hl
    ld l, a
    call _console_tx
    pop hl
    inc hl
    jr _console_puts

