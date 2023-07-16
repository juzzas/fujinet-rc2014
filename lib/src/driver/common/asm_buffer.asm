
PUBLIC asm_buffer_init
PUBLIC asm_buffer_tx_char
PUBLIC asm_buffer_tx_flush
PUBLIC asm_buffer_rx
PUBLIC asm_buffer_rx_avail

EXTERN fujinet_dcb_exec
EXTERN fujinet_poll_proceed

; context
DEFC CTX_OFFSET_BUFFER_ADDRL = 0  ; DEFW buffer_address (low byte)
DEFC CTX_OFFSET_BUFFER_ADDRH = 1  ; DEFW buffer_address (high byte)
DEFC CTX_OFFSET_BUFFER_LEN = 2   ; DEFB buffer length (max 64)
DEFC CTX_OFFSET_DEVICE_ID = 3    ; DEFB fujinet device_id
DEFC CTX_OFFSET_BUFFER_IDX = 4   ; DEFB current index of buffer

SECTION code_user

; Entry:
;   IX = context area
;   A = device ID
;   DE = 64-byte buffer
asm_buffer_init:
    ld (ix+CTX_OFFSET_BUFFER_ADDRL), e
    ld (ix+CTX_OFFSET_BUFFER_ADDRH), d
    ld (ix+CTX_OFFSET_BUFFER_LEN), 0
    ld (ix+CTX_OFFSET_DEVICE_ID), a
    ld (ix+CTX_OFFSET_BUFFER_IDX), 0
    ret

; local functions
; get index pointer in HL
; IX conext
get_context_buffer_index_ptr:
    ld a, (ix+CTX_OFFSET_BUFFER_IDX)
    ld h, (ix+CTX_OFFSET_BUFFER_ADDRH)
    ld l, (ix+CTX_OFFSET_BUFFER_ADDRL)
    add a, l    ; A = A+L
    ld l, a     ; L = A+L
    adc a, h    ; A = A+L+H+carry
    sub l       ; A = H+carry
    ld h, a     ; H = H+carry
    ret

inc_context_buffer_length:
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    inc a
    ld (ix+CTX_OFFSET_BUFFER_LEN), a
    ret

dec_context_buffer_length:
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    or a   ; check for underflow
    ret z

    dec a
    ld (ix+CTX_OFFSET_BUFFER_LEN), a
    ret

inc_context_buffer_index:
    ld a, (ix+CTX_OFFSET_BUFFER_IDX)
    inc a
    ld (ix+CTX_OFFSET_BUFFER_IDX), a
    ret


; Entry:
;   C = char to send
;   IX = context pointer
asm_buffer_tx_char:
    ; put character at index and increment index
    push hl
    call get_context_buffer_index_ptr
    ld (hl), c
    call inc_context_buffer_length
    call inc_context_buffer_index

    ; is buffer full?
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    cp 64
    call z, asm_buffer_tx_flush
    pop hl
    ret


; Send buffer to funjinet
; Entry:
;   IX = context pointer
asm_buffer_tx_flush:
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    or a
    ret z

;    uint8_t device;
    ld hl, buffer_fujinet_dcb
    ld a, (ix+CTX_OFFSET_DEVICE_ID)
    ld (hl), a
    inc hl

;    uint8_t command;
    ld (hl), 'W'
    inc hl

;    uint8_t aux1;
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    ld (hl), a
    inc hl

;    uint8_t aux2;
    xor a
    ld (hl), a
    inc hl

;    uint8_t *buffer;
    ld a, (ix+CTX_OFFSET_BUFFER_ADDRL)
    ld (hl), a
    inc hl
    ld a, (ix+CTX_OFFSET_BUFFER_ADDRH)
    ld (hl), a
    inc hl

;    uint16_t buffer_bytes; // data length in bytes
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    ld (hl), a
    inc hl
    xor a
    ld (hl), a
    inc hl

;    uint8_t *response;
    xor a
    ld (hl), a
    inc hl
    ld (hl), a
    inc hl

;    uint16_t response_bytes; // data length in bytes
    xor a
    ld (hl), a
    inc hl
    ld (hl), a
    inc hl

;    uint16_t timeout;   // milliseconds
    push de
    ld de, 15000
    ld (hl), e
    inc hl
    ld (hl), d
    inc hl
    pop de

    ; reset index
    xor a
    ld (ix+CTX_OFFSET_BUFFER_LEN), a
    ld (ix+CTX_OFFSET_BUFFER_IDX), a


    ; send to fujinet!
    push ix
    push de
    push bc
    ld hl, buffer_fujinet_dcb
    call fujinet_dcb_exec
    pop bc
    pop de
    pop ix

    ret

; Entry:
;   IX = context pointer
; Exit:
;    A = 0 (not available)
;    A = 255 (available)
asm_buffer_rx_avail:
    ; is there available data?
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    or a
    jr nz, rx_avail_yes

    ; no - check status -- 1 if asserted
    call fujinet_poll_proceed
    or a
    jr z, rx_avail_no

    ; if proceed, check data
    call do_buffer_rx_avail

    ; will there be available data?
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    or a
    jr z, rx_avail_no

    ; yes - do fetch
    call do_buffer_rx_read

rx_avail_yes:
    ld a, 255
    ret

rx_avail_no:
    ld a, 0
    ret


; Read character from buffer. Blocks until something is available.
; Entry:
;   IX = context pointer
; Exit:
;    A = character
asm_buffer_rx:
    call asm_buffer_rx_avail
    or a
    jr z, asm_buffer_rx

    ; something in the buffer
    push hl
    call get_context_buffer_index_ptr
    ld a, (hl)
    pop hl

    push af
    call inc_context_buffer_index
    call dec_context_buffer_length
    pop af
    ret



; Receive buffer from fujinet
;   -- assume CTX_OFFSET_BUFFER_LEN holds length of data to read
; Entry:
;   IX = context pointer
do_buffer_rx_read:
;    uint8_t device;
    ld hl, buffer_fujinet_dcb
    ld a, (ix+CTX_OFFSET_DEVICE_ID)
    ld (hl), a
    inc hl

;    uint8_t command;
    ld (hl), 'R'
    inc hl

;    uint8_t aux1;
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    ld (hl), a
    inc hl

;    uint8_t aux2;
    xor a
    ld (hl), a
    inc hl

;    uint8_t *buffer;
    xor a
    ld (hl), a
    inc hl
    ld (hl), a
    inc hl

;    uint16_t buffer_bytes; // data length in bytes
    xor a
    ld (hl), a
    inc hl
    ld (hl), a
    inc hl

;    uint8_t *response;
    ld a, (ix+CTX_OFFSET_BUFFER_ADDRL)
    ld (hl), a
    inc hl
    ld a, (ix+CTX_OFFSET_BUFFER_ADDRH)
    ld (hl), a
    inc hl

;    uint16_t response_bytes; // data length in bytes
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    ld (hl), a
    inc hl
    xor a
    ld (hl), a
    inc hl

;    uint16_t timeout;   // milliseconds
    push de
    ld de, 15000
    ld (hl), e
    inc hl
    ld (hl), d
    inc hl
    pop de

    ; send to fujinet!
    push ix
    push de
    push bc
    ld hl, buffer_fujinet_dcb
    call fujinet_dcb_exec
    pop bc
    pop de
    pop ix

    ret


; Receive availability from fujinet
;   -- assume CTX_OFFSET_BUFFER_LEN holds length of data to read
; Entry:
;   IX = context pointer
; Exit:
;   A = bytes remaining
do_buffer_rx_avail:
;    uint8_t device;
    ld hl, buffer_fujinet_dcb
    ld a, (ix+CTX_OFFSET_DEVICE_ID)
    ld (hl), a
    inc hl

;    uint8_t command;
    ld (hl), 'S'
    inc hl

;    uint8_t aux1;
    xor a
    ld (hl), a
    inc hl

;    uint8_t aux2;
    xor a
    ld (hl), a
    inc hl

;    uint8_t *buffer;
    xor a
    ld (hl), a
    inc hl
    ld (hl), a
    inc hl

;    uint16_t buffer_bytes; // data length in bytes
    xor a
    ld (hl), a
    inc hl
    ld (hl), a
    inc hl

;    uint8_t *response;
    push de
    ld de, buffer_fujinet_status
    ld (hl), e
    inc hl
    ld (hl), d
    inc hl
    pop de

;    uint16_t response_bytes; // data length in bytes
    ld a, 4
    ld (hl), a
    inc hl
    xor a
    ld (hl), a
    inc hl

;    uint16_t timeout;   // milliseconds
    push de
    ld de, 15000
    ld (hl), e
    inc hl
    ld (hl), d
    inc hl
    pop de

    ; send to fujinet!
    push ix
    push de
    push bc
    ld hl, buffer_fujinet_dcb
    call fujinet_dcb_exec
    pop bc
    pop de
    pop ix

    push de
    ; is it greater than 64?
    ld a, (buffer_fujinet_status + 0)
    ld l, a
    ld a, (buffer_fujinet_status + 1)
    ld h, a

    or a ; clear carry flag
    ld de, 64
    sbc hl, de
    add hl, de

    ; IF HL equals DE, Z=1,C=0
    ; IF HL is less than DE, Z=0,C=1
    ; IF HL is more than DE, Z=0,C=0
    jr c, rx_avail_lt64

rx_avail_64:
    ld (ix+CTX_OFFSET_BUFFER_LEN), 64
    ld (ix+CTX_OFFSET_BUFFER_IDX), 0
    pop de
    ret

rx_avail_lt64:
    ld a, (buffer_fujinet_status + 0) ; grab the LSB if less that 64 bytes
    ld (ix+CTX_OFFSET_BUFFER_LEN), a
    ld (ix+CTX_OFFSET_BUFFER_IDX), 0
    pop de
    ret



SECTION data_user

;struct fujinet_dcb {
;    uint8_t device;
;    uint8_t command;
;    uint8_t aux1;
;    uint8_t aux2;
;    uint8_t *buffer;
;    uint16_t buffer_bytes; // data length in bytes
;    uint8_t *response;
;    uint16_t response_bytes; // data length in bytes
;    uint16_t timeout;   // milliseconds
;};
buffer_fujinet_dcb:
    DEFS 14

buffer_fujinet_status:
    DEFS 4
