CFLAGS=-W -Wall -g

all: uspsv1 uspsv2 signal child_handler setitimer

uspsv1: uspsv1.o p1fxns.o
	gcc $(CFLAGS) -o uspsv1 uspsv1.o p1fxns.o

uspsv2: uspsv2.o p1fxns.o
	gcc $(CFLAGS) -o uspsv2 uspsv2.o p1fxns.o

uspsv3: uspsv3.o p1fxns.o
	gcc $(CFLAGS) -o uspsv3 uspsv3.o p1fxns.o

uspsv4: uspsv4.o p1fxns.o
	gcc $(CFLAGS) -o uspsv4 uspsv4.o p1fxns.o

signal: signal.o p1fxns.o
	gcc $(CFLAGS) -o signal signal.o p1fxns.o

child_handler: child_handler.o p1fxns.o
	gcc $(CFLAGS) -o child_handler child_handler.o p1fxns.o

setitimer: setitimer.o p1fxns.o
	gcc $(CFLAGS) -o setitimer setitimer.o p1fxns.o

clean:
	rm -f *.o uspsv1 uspsv2 uspsv3 signal child_handler setitimer

uspsv1.o: uspsv1.c p1fxns.h
uspsv2.o: uspsv2.c p1fxns.h
uspsv3.o: uspsv3.c p1fxns.h
uspsv4.o: uspsv4.c p1fxns.h
signal.o: signal.c p1fxns.h
child_handler.o: child_handler.c p1fxns.h
setitimer.o: setitimer.c p1fxns.h
p1fxns.o: p1fxns.c p1fxns.h
