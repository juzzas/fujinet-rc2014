<<<<<<< HEAD
#TARGET=+cpm
#TARGET=+rc2014 -subtype=cpm -clib=sdcc_iy --max-allocs-per-node200000
TARGET=+cpm -clib=sdcc_iy --max-allocs-per-node200000
=======
TARGET=+cpm -clib=sdcc_iy
#TARGET=+rc2014 -subtype=cpm -clib=sdcc_iy --max-allocs-per-node200000
>>>>>>> add loader
#SUB_TARGET=-Ilib/include @lib/lib_rc2014_cpm.lst
SUB_TARGET=-Ilib/include
LIB_FUJINET=@lib/lib_rc2014_cpm.lst
#TARGET=+rc2014 -subtype=hbios @hal/hal_rc2014_hbios.lst
#TARGET=+rc2014 -subtype=acia @hal/hal_rc2014_acia.lst
#TARGET=+rc2014 -subtype=sio @hal/hal_rc2014_sio.lst

#TARGET=+sam -subtype=highram @hal/hal_sam_acia.lst
#TARGET=+sam -subtype=highram @hal/hal_sam_sio.lst

# -subtype=allram    (default) Loads the program to A+B+C+D, screen paged on demand into C+D
# -subtype=highram   Loads the program to C+D, screen into A+B
# -subtype=basic     Uses MODE 1 from a BASIC environment (this subtype has much reduced functionality and use isn't recommended).


.PHONY: all clean lib fnreset fnwifi jsontest fnpip fnrsx

all: fninit fnreset

fninit:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fninit/fninit.lst ${LIB_FUJINET} -o fninit.com -create-app

fnreset:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fnreset/fnreset.lst ${LIB_FUJINET} -o fnreset.com -create-app

fnsetup:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fnsetup/fnsetup.lst ${LIB_FUJINET} -o fnsetup.com -create-app

fnwifi:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fnwifi/fnwifi.lst ${LIB_FUJINET} -o fnwifi.com -create-app

jsontest:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/jsontest/jsontest.lst ${LIB_FUJINET} -o jsontest.com -create-app

fnpip:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fnpip/fnpip.lst ${LIB_FUJINET} -o fnpip.com -create-app

fnstream:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fnstream/fnstream.lst ${LIB_FUJINET} -o fnstream.com -create-app

fndisk:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fndisk/fndisk.lst ${LIB_FUJINET} -o fndisk.com -create-app

fnrsx:
	z88dk-z80asm -v -b -reloc-info -l -s -m -g -ofnrsx_driver.bin @apps/fnrsx/fnrsx_driver.lst
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fnrsx/fnrsx.lst -o fnrsx.bin -create-app


clean:
	rm -f *.dsk *.map *.bin *.ihx *.com *.COM *.reloc *.def
	find . -name "*.lis" -exec rm {} \;
