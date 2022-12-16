;
; Created by jskists on 12/10/2022.
;

EXTERN fujinet_init
EXTERN fujinet_dcb_exec

PUBLIC _fujinet_init
PUBLIC _fujinet_dcb_exec

SECTION code_user

;void fujinet_init(void)
_fujinet_init:
    push iy
    push ix
    push de
    push bc
    call fujinet_init
    pop bc
    pop de
    pop ix
    pop iy
    ret



;FUJINET_RC fujinet_dcb_exec(struct fujinet_dcb *dcb)

_fujinet_dcb_exec:
    push iy
    push ix
    push de
    push bc
    call fujinet_dcb_exec
    ld h, 0
    ld l, a
    pop bc
    pop de
    pop ix
    pop iy
    ret



