PUBLIC _main

EXTERN asm_drv_printer_init
EXTERN asm_drv_printer_put_char
EXTERN asm_drv_printer_flush

section code_user

_main:
    call asm_drv_printer_init


    ld c, 65
    call asm_drv_printer_put_char


    call asm_drv_printer_flush

    ret
