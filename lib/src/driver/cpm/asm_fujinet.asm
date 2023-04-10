;
; Created by jskists on 12/10/2022.
;

PUBLIC fujinet_init
PUBLIC fujinet_dcb_exec
PUBLIC fujinet_poll_proceed

DEFC BDOS = 5
DEFC FNC_DCB = 0xf0
DEFC FNC_POLL = 0xf1


SECTION code_user

;void fujinet_init(void) {
;    fujinet_hal_init();
;}
fujinet_init:
    ld c, FNC_DCB
    ld de, 0
    jp BDOS


;FUJINET_RC fujinet_dcb_exec(struct fujinet_dcb *dcb) {

; ENTRY: HL = dcb
; USES:  IX = pointer to dcb
; EXIT:   A = FUJINET_RC

fujinet_dcb_exec:
    ld c, FNC_DCB
    ld de, hl
    jp BDOS

fujinet_poll_proceed:
    ld c, FNC_POLL
    jp BDOS
