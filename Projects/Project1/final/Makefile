CFLAGS=-W -Wall -g

all: usps signal setitimer

usps: usps.o p1fxns.o
	gcc $(CFLAGS) -o usps usps.o p1fxns.o

signal: signal.o p1fxns.o
	gcc $(CFLAGS) -o signal signal.o p1fxns.o

setitimer: setitimer.o p1fxns.o
	gcc $(CFLAGS) -o setitimer setitimer.o p1fxns.o

clean:
	rm -f *.o usps signal setitimer

usps.o: usps.c p1fxns.h
signal.o: signal.c p1fxns.h
setitimer.o: setitimer.c p1fxns.h
p1fxns.o: p1fxns.c p1fxns.h