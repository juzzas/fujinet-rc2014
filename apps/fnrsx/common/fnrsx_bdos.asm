; handles the overriding of BDOS functions

EXTERN asm_drv_printer_put_char

PUBLIC handle_bdos_l_write

; BDOS function 5 (L_WRITE) - Printer output
;  Supported by: All versions
;  Entered with C=5, E=ASCII character.
;
;  If the printer is permanently offline or busy, this call can hang.
handle_bdos_l_write:
    jp asm_drv_printer_put_char

