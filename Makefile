TARGET=+cpm
#SUB_TARGET=-Ilib/include @lib/lib_rc2014_cpm.lst
SUB_TARGET=-Ilib/include
#TARGET=+rc2014 -subtype=hbios @hal/hal_rc2014_hbios.lst
#TARGET=+rc2014 -subtype=acia @hal/hal_rc2014_acia.lst
#TARGET=+rc2014 -subtype=sio @hal/hal_rc2014_sio.lst

#TARGET=+sam -subtype=highram @hal/hal_sam_acia.lst
#TARGET=+sam -subtype=highram @hal/hal_sam_sio.lst

# -subtype=allram    (default) Loads the program to A+B+C+D, screen paged on demand into C+D
# -subtype=highram   Loads the program to C+D, screen into A+B
# -subtype=basic     Uses MODE 1 from a BASIC environment (this subtype has much reduced functionality and use isn't recommended).


.PHONY: all clean lib fnreset fnwifi

all: fnreset fnwifi fnstream

fnreset:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fnreset/fnreset.lst @lib/lib_rc2014_cpm.lst -o fnreset.com -create-app

fnsetup:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fnsetup/fnsetup.lst @lib/lib_rc2014_cpm.lst -o fnsetup.com -create-app

fnwifi:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fnwifi/fnwifi.lst @lib/lib_rc2014_cpm.lst -o fnwifi.com -create-app

jsontest:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/jsontest/jsontest.lst @lib/lib_rc2014_cpm.lst -o jsontest.com -create-app

wget:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/wget/wget.lst @lib/lib_rc2014_cpm.lst -o wget.com -create-app

fnstream:
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  @apps/fnstream/fnstream.lst @lib/lib_rc2014_cpm.lst -o fnstream.com -create-app



clean:
	rm -f *.dsk *.map *.bin *.ihx *.com *.COM
	find . -name "*.lis" -exec rm {} \;
