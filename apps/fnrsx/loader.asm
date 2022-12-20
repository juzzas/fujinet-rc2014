;;------------------------------------------------------------------------
PUBLIC _driver_code           ; extern uint8_t* driver_code
PUBLIC _driver_code_size      ; extern uint16_t driver_code_size
PUBLIC _reloc_info            ; extern uint16_t* reloc_info
PUBLIC _reloc_info_size      ; extern uint16_t reloc_info_size
PUBLIC _driver_code_init

SECTION rodata_user

_driver_code:
    DEFW start_driver_code

_driver_code_size:
    DEFW (end_driver_code - start_driver_code)

_reloc_info:
    DEFW start_reloc_info

_reloc_info_size:
    DEFW ((end_reloc_info - start_reloc_info) / 2)

_driver_code_init:
    jp (hl)
    ret



start_driver_code:
    INCBIN "fnrsx_driver.bin"
end_driver_code:

;; array of 16bit address values that need to be patched (read, add start address, write)
start_reloc_info:
    INCBIN "fnrsx_driver.reloc"
end_reloc_info:

