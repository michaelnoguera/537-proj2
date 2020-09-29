# See README for notes about project organization

DEPS=queue.h
CFLAGS=-std=c99 -Wall

.PHONY:clean

all: prodcomm

prodcomm: main.o queue.o
	gcc -o 537ps main.o queue.o

%.o: %.c $(DEPS)
ifeq ($(DEBUG),true)
	scan-build gcc -g -c -o $@ $< $(CFLAGS)
else
	gcc -c -o $@ $< $(CFLAGS)
endif

clean:
	rm *.o
	rm prodcomm
