#TARGET=+cpm
TARGET=+rc2014 -subtype=cpm -clib=sdcc_iy
CFLAGS=--max-allocs-per-node200000 -SO3 -Ilib/include
#TARGET=+cpm -clib=sdcc_iy --max-allocs-per-node200000

SUB_TARGET=-Ilib/include
LIB_DRV_FUJINET=-libfn_drv
LIB_FUJINET=-llibfn_cpm -llibfujinet
LIB_FUJINET_ACIA=-llibfn_acia -llibfujinet

#TARGET=+rc2014 -subtype=hbios @hal/hal_rc2014_hbios.lst
#TARGET=+rc2014 -subtype=acia @hal/hal_rc2014_acia.lst
#TARGET=+rc2014 -subtype=sio @hal/hal_rc2014_sio.lst

#TARGET=+sam -subtype=highram @hal/hal_sam_acia.lst
#TARGET=+sam -subtype=highram @hal/hal_sam_sio.lst

# -subtype=allram    (default) Loads the program to A+B+C+D, screen paged on demand into C+D
# -subtype=highram   Loads the program to C+D, screen into A+B
# -subtype=basic     Uses MODE 1 from a BASIC environment (this subtype has much reduced functionality and use isn't recommended).
CPU_CLOCK=7372800

.PHONY: all clean libs fnreset fnwifi fnpip fnrsx fnrsx22 fnmon fnmon-rom camelforth

all: fninit fnreset

libs: libfn_drv libfn_cpm libfn_acia libfujinet

libfn_drv:
	zcc ${TARGET} -v -x ${CFLAGS} @lib/driver_rc2014_spi.lst -o libfn_drv -create-app

libfn_cpm:
	zcc ${TARGET} -v -x ${CFLAGS} @lib/driver_rc2014_cpm.lst -o libfn_cpm -create-app

libfn_acia:
	zcc ${TARGET} -v -x ${CFLAGS} @lib/driver_rc2014_acia.lst -o libfn_acia -create-app

libfujinet:
	zcc ${TARGET} -v -x ${CFLAGS} @lib/libfujinet/libfujinet_sdcc_iy.lst -o libfujinet -create-app

fnreset:
	zcc ${TARGET} -v -m --list ${CFLAGS} ${LIB_FUJINET} @apps/fnreset/fnreset.lst -o fnreset -create-app

fnwifi:
	zcc ${TARGET} -v -m --list ${CFLAGS} ${LIB_FUJINET} @apps/fnwifi/fnwifi.lst -o fnwifi -create-app

fnpip:
	zcc ${TARGET} -v -m --list ${CFLAGS} ${LIB_FUJINET} @apps/fnpip/fnpip.lst -o fnpip -create-app

fnstream:
	zcc ${TARGET} -v -m --list ${CFLAGS} ${LIB_FUJINET} @apps/fnstream/fnstream.lst -o fnstream -create-app

fndisk:
	zcc ${TARGET} -v -m --list ${CFLAGS} ${LIB_FUJINET} @apps/fndisk/fndisk.lst -o fndisk -create-app

fnrsx:
	z88dk-z80asm -v -b -reloc-info -l -s -m -g -D__CPU_CLOCK=${CPU_CLOCK} -llibfn_drv -ofnrsx_driver.bin @apps/fnrsx/fnrsx_driver.lst
	reloc2prl -r fnrsx_driver.reloc -o fnrsx.rsx fnrsx_driver.bin
	zcc ${TARGET} -v -m --list  @apps/fnrsx/fnrsx.lst -o fnrsx -create-app

fnrsx22:
	z88dk-z80asm -v -b -reloc-info -l -s -m -g -D__CPU_CLOCK=${CPU_CLOCK} -DCPM_22 -llibfn_drv -ofnrsx_driver22.bin @apps/fnrsx/fnrsx_driver.lst
	reloc2prl -r fnrsx_driver22.reloc -o fnrsx22.prl fnrsx_driver22.bin
	sync
	zcc ${TARGET} -v -m --list @apps/fnrsx/fnrsx_ldr22.lst -o fnrsx22 -create-app
	cp fnrsx22.bin fnrsx22.com

fnlogin:
	zcc ${TARGET} -v -m --list ${CFLAGS} ${LIB_FUJINET} @apps/fnlogin/fnlogin.lst -o fnlogin -create-app

ptest:
	zcc ${TARGET} -v -m --list ${CFLAGS} ${LIB_FUJINET} @apps/printtest/printtest.lst -o ptest -create-app

modem:
	zcc ${TARGET} -v -m --list ${CFLAGS} ${LIB_FUJINET} @apps/modemtest/modem.lst -o modem -create-app

fnmon:
	zcc +rc2014 -subtype=cpm -clib=sdcc_iy ${CFLAGS} -v -m --list ${LIB_FUJINET} @apps/fnmon/fnmon.lst -o fnmon -create-app

fnmon-rom:
	zcc +embedded -startup=1 -clib=sdcc_iy ${CFLAGS} -v -m --list -pragma-include:apps/fnmon/zpragma.inc ${LIB_FUJINET_ACIA}  @apps/fnmon/fnmon-rom.lst -o fnmon-rom -create-app

camelforth:
	z88dk-z80asm -b -v -l -s -m -g -D__CPU_CLOCK=${CPU_CLOCK} -llibfn_drv -ofncamelforth.bin @camelforth/camelforth_drv.lst
	z88dk-appmake +hex -v --org=0xf800 -b fncamelforth.bin
#	zcc +embedded -subytpe=none --no-crt -clib=sdcc_iy ${CFLAGS} -D__CPU_CLOCK=${CPU_CLOCK} -llibfn_drv @camelforth/camelforth_drv.lst -o camelforth_drv -create-app -Cz"--org 0xf800 --ihex"

clean:
	rm -f *.dsk *.map *.bin *.ihx *.com *.COM *.reloc *.def *.prl *.rsx *.lib
	find . -name "*.lis" -exec rm {} \;
	find . -name "*.sym" -exec rm {} \;
	find . -name "*.o" -exec rm {} \;
