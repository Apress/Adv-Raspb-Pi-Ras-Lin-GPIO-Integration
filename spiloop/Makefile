CC	= gcc
OPTS	= -Wall
DBG	= -O0 -g
CFLAGS	= $(OPTS) $(DBG)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $*.o

OBJS	= spiloop.o

all:	$(OBJS)
	$(CC) $(OBJS) -o spiloop
	sudo chown root ./spiloop
	sudo chmod u+s ./spiloop

clean:
	rm -f *.o core .errs.t

clobber: clean
	rm -f spiloop
