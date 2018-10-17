CC	= gcc
OPTS	= -Wall
DBG	= -O0 -g
CFLAGS	= $(OPTS) $(DBG)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $*.o

OBJS=dht11.o libgp.o

all:	$(OBJS)
	$(CC) $(OBJS) -o dht11 -lpthread
	sudo chown root ./dht11
	sudo chmod u+s ./dht11

libgp.o: CFLAGS += -O3
dht11.o: CFLAGS += -O3

clean:
	rm -f *.o core errs.t

clobber: clean
	rm -f dht11

