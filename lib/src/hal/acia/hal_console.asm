
DEFC BDOS = 0x0005

SECTION code_user

PUBLIC _console_init, _console_tx, _console_rx, _console_rx_avail, _console_puts
EXTERN _acia_putc, _acia_getc, _acia_pollc

;;Initialised the FujiNet hardware

_console_init:
    ret


;; Output byte from Console device
; entry:
;    L = byte
_console_tx:
    jp _acia_putc


;; Input byte from Console device
; exit:
;    L = byte
_console_rx:
    jp _acia_getc


;; Number of bytes available in receive buffer
; exit:
;    L = number of bytes in buffer
_console_rx_avail:
    jp _acia_pollc


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

