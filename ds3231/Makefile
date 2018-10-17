CC	= gcc
OPTS	= -Wall
DBG	= -O0 -g
CFLAGS	= $(OPTS) $(DBG)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $*.o

all:	ds3231

ds3231: ds3231.o
	$(CC) ds3231.o -o ds3231
	sudo chown root ./ds3231
	sudo chmod u+s ./ds3231

clean:
	rm -f *.o core errs.t

clobber: clean
	rm -f ds3231

