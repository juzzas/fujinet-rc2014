
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
    push de
    ld c, 6     ; CP/M 3 C_WRITE (Returns A=byte)
    ld e, l
    push ix
    push iy
    call BDOS
    pop iy
    pop ix
    pop de
    pop bc
    ret


;; Input byte from Console device
; exit:
;    L = byte
_console_rx:
    ld a, (last_char)
    or a

    jr nz, console_rx_1

    ; else try again
    call z, console_rx_try
    jr _console_rx

console_rx_1:

    ld l, a
    xor a
    ld (last_char), a

    ret


;; Number of bytes available in receive buffer
; exit:
;    L = number of bytes in buffer
_console_rx_avail:
    ld a, (last_char)
    or a
    jr nz, rx_avail_yes

    ; zero means no key waiting
    call console_rx_try

    ld a, (last_char)
    or a
    jr nz, rx_avail_yes

rx_avail_no:
    ld l, 0
    ret

rx_avail_yes:
    ld l, 1   ; let's just say that there is only one byte.
    ret


console_rx_try:
    push bc
    push de
    ld c, 6    ; CP/M C_READ (Returns A=byte)
    ld e, 0xff

    push ix
    push iy
    call BDOS
    pop iy
    pop ix

    ld (last_char), a

    pop de
    pop bc
    ret

last_char:
    DEFB 0


_console_puts:
    ld a, (hl)
    or a
    ret z

    push bc
    push de
    push hl
    push ix
    push iy
    ld c, 6     ; CP/M 3 C_WRITE (Returns A=byte)
    ld e, a
    call BDOS
    pop iy
    pop ix
    pop hl
    pop de
    pop bc
    inc hl
    jr _console_puts

