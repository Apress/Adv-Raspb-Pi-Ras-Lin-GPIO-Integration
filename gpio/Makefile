CC	= gcc
OPTS	= -Wall
DBG	= -O0 -g
CFLAGS	= $(OPTS) $(DBG)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $*.o

OBJS	= gp.o

all:	$(OBJS)
	$(CC) $(OBJS) -o gp
	sudo chown root ./gp
	sudo chmod u+s ./gp

clean:
	rm -f *.o core .errs.t

clobber: clean
	rm -f gp
