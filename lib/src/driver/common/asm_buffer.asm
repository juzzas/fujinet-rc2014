
PUBLIC asm_buffer_init
PUBLIC asm_buffer_tx_char
PUBLIC asm_buffer_tx_flush

EXTERN fujinet_dcb_exec

; context
DEFC CTX_OFFSET_BUFFER_ADDRL = 0  ; DEFW buffer_address (low byte)
DEFC CTX_OFFSET_BUFFER_ADDRH = 1  ; DEFW buffer_address (high byte)
DEFC CTX_OFFSET_BUFFER_LEN = 2   ; DEFB buffer length (max 64)
DEFC CTX_OFFSET_DEVICE_ID = 3    ; DEFB fujinet device_id

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
    ret

; local functions
; get index pointer in HL
; IX conext
get_context_buffer_index_ptr:
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    ld h, (ix+CTX_OFFSET_BUFFER_ADDRH)
    ld l, (ix+CTX_OFFSET_BUFFER_ADDRL)
    add a, l    ; A = A+L
    ld l, a     ; L = A+L
    adc a, h    ; A = A+L+H+carry
    sub l       ; A = H+carry
    ld h, a     ; H = H+carry
    ret

inc_context_buffer_index:
    ld a, (ix+CTX_OFFSET_BUFFER_LEN)
    inc a
    ld (ix+CTX_OFFSET_BUFFER_LEN), a
    ret


; Entry:
;   C = char to send
;   IX = context pointer
asm_buffer_tx_char:
    ; put character at index and increment index
    push hl
    call get_context_buffer_index_ptr
    ld (hl), c
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