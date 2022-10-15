TARGET=+rc2014
SUB_TARGET=-subtype=cpm -Ihal/include @hal/hal_rc2014_cpm.lst
#TARGET=+rc2014 -subtype=hbios @hal/hal_rc2014_hbios.lst
#TARGET=+rc2014 -subtype=acia @hal/hal_rc2014_acia.lst
#TARGET=+rc2014 -subtype=sio @hal/hal_rc2014_sio.lst

#TARGET=+sam -subtype=highram @hal/hal_sam_acia.lst
#TARGET=+sam -subtype=highram @hal/hal_sam_sio.lst

# -subtype=allram    (default) Loads the program to A+B+C+D, screen paged on demand into C+D
# -subtype=highram   Loads the program to C+D, screen into A+B
# -subtype=basic     Uses MODE 1 from a BASIC environment (this subtype has much reduced functionality and use isn't recommended).

.PHONY: all clean

all: fntest

fntest: fntest.c
	zcc ${TARGET} -v -m --list ${SUB_TARGET}  fntest.c fujinet.c -o fntest -create-app



clean:
	rm -f *.dsk *.map *.bin *.ihx *.lis
