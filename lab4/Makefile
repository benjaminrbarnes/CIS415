# Rickie Kerndt <rkerndt@uoregon.edu>
# CIS 415 Spring 2014
# Assignment 2, Problem 1 (6.40)

CC= gcc
CFLAGS= -g -std=gnu99 -W -Wall
LFLAGS= -lpthread
TARGETS= lab4

all: ${TARGETS}

lab4: lab4.c
	${CC} ${CFLAGS} -o lab4 lab4.c ${LFLAGS}

clean:
	rm -f *.o ${TARGETS}

.PHONY: clean
