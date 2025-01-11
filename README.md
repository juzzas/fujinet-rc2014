# FujiNet -- designed for RC2014

## What is it?

FujiNet is a multi-peripheral emulator and WiFi network device for vintage computers.

https://fujinet.online

This is the RC2014 library pack for it.

## What is RC2014?

RC2014 is a modular computer system created by Spencer Owen.

https://rc2014.co.uk

## Drivers

### ZPM3 and CP/M 3 RSX

The RSX is a resident driver that adds additional functions to BDOS.

Compiling the RSX driver requires the reloc2prl tool: https://github.com/juzzas/reloc2prl

The RSX adds two functions to BDOS:

0xF0 -- Fujinet DCB command
```
; entry: de = address of FujiNet DCB command
; uses: ix, bc, hl, a, a'
; exit: a = return code (FUJINET_RC)
```

0xF1 -- Fujinet poll command
```
; entry: none
; uses: ix, bc, hl, a, a'
; exit: a = 1: asserted, 0: not asserted
```


#### installation

Copy fnrsx binary to the CP/M system as FNRSX.COM

Copy fnrsx.rsx file to CP/M system as FNRSX.RSX

Then type the following commands into CP/M prompt:

```
gencom fnxrsx.com fnrsx
```

Load the RSX with

```
fnrsx
```

## Warning

This library pack is still in development, and is most definitely not yet ready for general use.
