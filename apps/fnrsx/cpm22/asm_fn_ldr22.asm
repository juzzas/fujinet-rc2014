
PUBLIC _fn_payload
PUBLIC _fn_payload_size

SECTION rodata_user

_fn_payload:
    DEFW fn_payload

_fn_payload_size:
    DEFW fn_payload_end - fn_payload

fn_payload:
    INCBIN "../../../fnrsx22.prl"
fn_payload_end:

