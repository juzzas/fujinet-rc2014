; chain-loads a program

DEFC BDOS = 0x0005

DEFC BDOS_F_OPEN = 0x0F
DEFC BDOS_F_CLOSE = 0x10
DEFC BDOS_F_READ = 0x14
DEFC BDOS_F_DMAOFF = 0x1A
DEFC BDOS_DRV_GET = 0x19

DEFC TFCB = 0x5C		;default file control block.
DEFC TBUFF = 0x80		;i/o buffer and command line storage.

PUBLIC chain_load

; DE = chain flag
;   Process command line at 0x80 here.
chain_load:
    LD	DE,TBUFF
	CALL	DMASET		;set standard dma address.
	CALL	GETDSK
	LD	(CDRIVE),A	;set current drive.
	CALL	CONVFST		;convert name typed in.
	CALL	NZ,SYNERR	;wild cards are not allowed.
	JP	NZ, LOAD_CMD	;which gets executed.

; only return on error
CMMND1:
	RET

;
;   Convert the first name in (FCB).
;
CONVFST:LD	A,0
;
;   Format a file name (convert * to '?', etc.). On return,
; (A)=0 is an unambigeous name was specified. Enter with (A) equal to
; the position within the fcb for the name (either 0 or 16).
;
CONVERT:LD	HL,FCB
	CALL	ADDHL
	PUSH	HL
	PUSH	HL
	XOR	A
	LD	(CHGDRV),A	;initialize drive change flag.
	LD	HL,(INPOINT)	;set (HL) as pointer into input line.
	EX	DE,HL
	CALL	NONBLANK	;get next non-blank character.
	EX	DE,HL
	LD	(NAMEPNT),HL	;save pointer here for any error message.
	EX	DE,HL
	POP	HL
	LD	A,(DE)		;get first character.
	OR	A
	JP	Z,CONVRT1
	SBC	A,'A'-1		;might be a drive name, convert to binary.
	LD	B,A		;and save.
	INC	DE		;check next character for a ':'.
	LD	A,(DE)
	CP	':'
	JP	Z,CONVRT2
	DEC	DE		;nope, move pointer back to the start of the line.
CONVRT1:LD	A,(CDRIVE)
	LD	(HL),A
	JP	CONVRT3
CONVRT2:LD	A,B
	LD	(CHGDRV),A	;set change in drives flag.
	LD	(HL),B
	INC	DE
;
;   Convert the basic file name.
;
CONVRT3:LD	B,08H
CONVRT4:CALL	CHECK
	JP	Z,CONVRT8
	INC	HL
	CP	'*'		;note that an '*' will fill the remaining
	JP	NZ,CONVRT5	;field with '?'.
	LD	(HL),'?'
	JP	CONVRT6
CONVRT5:LD	(HL),A
	INC	DE
CONVRT6:DEC	B
	JP	NZ,CONVRT4
CONVRT7:CALL	CHECK		;get next delimiter.
	JP	Z,GETEXT
	INC	DE
	JP	CONVRT7
CONVRT8:INC	HL		;blank fill the file name.
	LD	(HL),' '
	DEC	B
	JP	NZ,CONVRT8
;
;   Get the extension and convert it.
;
GETEXT:	LD	B,03H
	CP	'.'
	JP	NZ,GETEXT5
	INC	DE
GETEXT1:CALL	CHECK
	JP	Z,GETEXT5
	INC	HL
	CP	'*'
	JP	NZ,GETEXT2
	LD	(HL),'?'
	JP	GETEXT3
GETEXT2:LD	(HL),A
	INC	DE
GETEXT3:DEC	B
	JP	NZ,GETEXT1
GETEXT4:CALL	CHECK
	JP	Z,GETEXT6
	INC	DE
	JP	GETEXT4
GETEXT5:INC	HL
	LD	(HL),' '
	DEC	B
	JP	NZ,GETEXT5
GETEXT6:LD	B,3
GETEXT7:INC	HL
	LD	(HL),0
	DEC	B
	JP	NZ,GETEXT7
	EX	DE,HL
	LD	(INPOINT),HL	;save input line pointer.
	POP	HL
;
;   Check to see if this is an ambigeous file name specification.
; Set the (A) register to non zero if it is.
;
	LD	BC,11		;set name length.
GETEXT8:INC	HL
	LD	A,(HL)
	CP	'?'		;any question marks?
	JP	NZ,GETEXT9
	INC	B		;count them.
GETEXT9:DEC	C
	JP	NZ,GETEXT8
	LD	A,B
	OR	A
	RET

;**************************************************************
;*
;*        T R A N S I A N T   P R O G R A M   C O M M A N D
;*
;**************************************************************
;
LOAD_CMD:
	LD	A,(FCB+1)	;anything to execute?
	CP	' '
	JP	NZ,UNKWN1
	JP	GETBACK1	;then return.
;
;   Here a file name was typed. Prepare to execute it.
;
UNKWN1:
    LD	DE,FCB+9	;an extension specified?
	LD	A,(DE)
	CP	' '
	JP	NZ,SYNERR	;yes, not allowed.
UNKWN2:
    PUSH	DE
	CALL	DSELECT		;select specified drive.
	POP	DE
	LD	HL,COMFILE	;set the extension to 'COM'.
	CALL	MOVE3
	CALL	OPENFCB		;and open this file.
	JP	Z,UNKWN9	;not present?
;
;   Load in the program.
;
	LD	HL,TBASE	;store the program starting here.
UNKWN3:
    PUSH	HL
	EX	DE,HL
	CALL	DMASET		;set transfer address.
	LD	DE,FCB		;and read the next record.
	CALL	RDREC
	JP	NZ,UNKWN4	;end of file or read error?
	POP	HL		;nope, bump pointer for next sector.
	LD	DE,128
	ADD	HL,DE
	LD	DE,CBASE	;enough room for the whole file?
	LD	A,L
	SUB	E
	LD	A,H
	SBC	A,D
	JP	NC,UNKWN0	;no, it can't fit.
	JP	UNKWN3
;
;   Get here after finished reading.
;
UNKWN4:	POP	HL
	DEC	A		;normal end of file?
	JP	NZ,UNKWN0
	CALL	RESETDR		;yes, reset previous drive.
	CALL	CONVFST		;convert the first file name that follows
	LD	HL,CHGDRV	;command name.
	PUSH	HL
	LD	A,(HL)		;set drive code in default fcb.
	LD	(FCB),A
	LD	A,16		;put second name 16 bytes later.
	CALL	CONVERT		;convert second file name.
	POP	HL
	LD	A,(HL)		;and set the drive for this second file.
	LD	(FCB+16),A
	XOR	A		;clear record byte in fcb.
	LD	(FCB+32),A
	LD	DE,TFCB		;move it into place at(005Ch).
	LD	HL,FCB
	LD	B,33
	CALL	HL2DE
	LD	HL,INBUFF+2	;now move the remainder of the input
UNKWN5:	LD	A,(HL)		;line down to (0080h). Look for a non blank.
	OR	A		;or a null.
	JP	Z,UNKWN6
	CP	' '
	JP	Z,UNKWN6
	INC	HL
	JP	UNKWN5
;
;   Do the line move now. It ends in a null byte.
;
UNKWN6:	LD	B,0		;keep a character count.
	LD	DE,TBUFF+1	;data gets put here.
UNKWN7:	LD	A,(HL)		;move it now.
	LD	(DE),A
	OR	A
	JP	Z,UNKWN8
	INC	B
	INC	HL
	INC	DE
	JP	UNKWN7
UNKWN8:	LD	A,B		;now store the character count.
	LD	(TBUFF),A
	CALL	CRLF		;clean up the screen.
	CALL	STDDMA		;set standard transfer address.
	CALL	SETCDRV		;reset current drive.
	CALL	TBASE		;and execute the program.
;
;   Transiant programs return here (or reboot).
;
	CALL	MOVECD		;move current drive into place (TDRIVE).
	CALL	DSKSEL		;and reselect it.
	JP	CMMND1		;back to comand mode.
;
;   Get here if some error occured.
;
UNKWN9:	CALL	RESETDR		;inproper format.
	JP	SYNERR
UNKWN0:	LD	BC,BADLOAD	;read error or won't fit.
	CALL	PLINE
	JP	GETBACK
BADLOAD:.TEXT	"Bad load"
	.DB	0
COMFILE:.TEXT	"COM"		;command file extension.

;
;   Get here to return to command level. We will reset the
; previous active drive and then either return to command
; level directly or print error message and then return.
;
GETBACK:CALL	RESETDR		;reset previous drive.
GETBACK1: CALL	CONVFST		;convert first name in (FCB).
	LD	A,(FCB+1)	;if this was just a drive change request,
	SUB	' '		;make sure it was valid.
	LD	HL,CHGDRV
	OR	(HL)
	JP	NZ,SYNERR
	JP	CMMND1		;ok, return to command level.


;
;   Print back file name with a '?' to indicate a syntax error.
;
SYNERR:	CALL	CRLF		;end current line.
	LD	HL,(NAMEPNT)	;this points to name in error.
SYNERR1:LD	A,(HL)		;print it until a space or null is found.
	CP	' '
	JP	Z,SYNERR2
	OR	A
	JP	Z,SYNERR2
	PUSH	HL
	CALL	PRINT
	POP	HL
	INC	HL
	JP	SYNERR1
SYNERR2:LD	A,'?'		;add trailing '?'.
	CALL	PRINT
	CALL	CRLF
	JP	CMMND1		;and restart from console input.
;
;   Check character at (DE) for legal command input. Note that the
; zero flag is set if the character is a delimiter.
;
CHECK:	LD	A,(DE)
	OR	A
	RET	Z
	CP	' '		;control characters are not legal here.
	JP	C,SYNERR
	RET	Z		;check for valid delimiter.
	CP	'='
	RET	Z
	CP	'_'
	RET	Z
	CP	'.'
	RET	Z
	CP	':'
	RET	Z
	CP	03BH ; ';'
	RET	Z
	CP	'<'
	RET	Z
	CP	'>'
	RET	Z
	RET
;
;   Get the next non-blank character from (DE).
;
NONBLANK: LD	A,(DE)
	OR	A		;string ends with a null.
	RET	Z
	CP	' '
	RET	NZ
	INC	DE
	JP	NONBLANK
;
;   Add (HL)=(HL)+(A)
;
ADDHL:	ADD	A,L
	LD	L,A
	RET	NC		;take care of any carry.
	INC	H
	RET

DMASET:
    LD C, BDOS_F_DMAOFF
    JP BDOS

GETDSK:
    LD	C, BDOS_DRV_GET
	JP	BDOS


CDRIVE:
    DB 0