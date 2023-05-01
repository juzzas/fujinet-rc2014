; chain-loads a program

DEFC BDOS = 0x0005

DEFC BDOS_F_OPEN = 0x0F
DEFC BDOS_F_CLOSE = 0x10
DEFC BDOS_F_READ = 0x14
DEFC BDOS_SET_DMAOFF = 0x1A

PUBLIC chain_load

; DE = fully qualified FCB
chain_load:
    ; copy fcb
    ld hl, de
    ld de, fbc_copy
    ld b, 36
    ldir
    ld de, fbc_copy

    ; open file
    call chain_open
    jr c, chain_error

    call do_chain_load

    call chain_close

    jp 0x100

chain_error:
    ld a, 1
    ret

; ============================================================================
; DE = fully qualified FCB
chain_open:
    ld de, fbc_copy
    ld c, BDOS_F_OPEN
    call BDOS
    cp 0xff
    jr z, chain_open_error
    or a; clear carry flag
    ret

chain_open_error:
    scf
    ret

; ============================================================================
; DE = fully qualified FCB
chain_close:
    ld de, fbc_copy
    ld c, BDOS_F_CLOSE
    call BDOS
    ret

; ============================================================================
; DE = fully qualified FCB
chain_load_sector:
    ld de, fbc_copy
    ld c, BDOS_F_READ
    call BDOS
    ret

do_chain_load:
    call init_dma

do_chan_load_loop:
    call chain_load_sector
    cp 0
    jr nz, end_load

    call inc_dma
    jr do_chan_load_loop

end_load:
    ret

; ============================================================================

init_dma:
    push de
    ld de, 0x100
    ld (dma_addr), de
    ld c, BDOS_SET_DMAOFF
    call BDOS
    pop de
    ret

inc_dma:
    push hl
    push de
    ld hl, (dma_addr)
    ld de, 128
    add hl, de
    ld (dma_addr), hl
    ld d, h
    ld e, l
    ld c, BDOS_SET_DMAOFF
    call BDOS
    pop de
    pop hl
    ret

dma_addr:
    DEFW 0x100

fbc_copy:
    DEFS 36