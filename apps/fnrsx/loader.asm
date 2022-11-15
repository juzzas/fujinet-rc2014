;;------------------------------------------------------------------------
PUBLIC _driver_code           ; extern uint8_t* driver_code
PUBLIC _driver_code_size      ; extern uint16_t driver_code_size
PUBLIC _reloc_info            ; extern uint16_t* reloc_info
PUBLIC _reloc_info_size      ; extern uint16_t reloc_info_size

_driver_code:
    INCBIN "fnrsx.bin"
end_driver_code:

_driver_code_size:
    DEFW (end_driver_code -_driver_code)

;; array of 16bit address values that need to be patched (read, add start address, write)
_reloc_info:
    INCBIN "fnrsx.reloc"
end_reloc_info:

_reloc_info_size:
    DEFW ((end_reloc_info - _reloc_info) / 2)
