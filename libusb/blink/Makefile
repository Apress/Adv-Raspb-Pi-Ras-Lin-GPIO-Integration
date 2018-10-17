BUS	?= 001
DEV	?= 005

all:	blink

blink:
	$(CC) $(CFLAGS) blink.c

list:
	$(PROG) --list

prog:	blink.ihx
	sudo $(PROG) -d=$(BUS).$(DEV) reset prg:blink.ihx run

reset:
	sudo $(PROG) -d=$(BUS).$(DEV) reset

clobber: clean
	rm -f blink.ihx .errs.t

include ../ezusb/Makefile.incl

