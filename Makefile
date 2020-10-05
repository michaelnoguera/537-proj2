# See README for notes about project organization

DEPS=queue.h
CFLAGS=-std=c99 -Wall

.PHONY:clean

all: prodcomm

prodcomm: main.o queue.o workers.o
	scan-build gcc -o prodcomm main.o queue.o workers.o -pthread

%.o: %.c $(DEPS)
ifeq ($(DEBUG),true)
	gcc -g -c -o $@ $< $(CFLAGS)
else
	gcc -c -o $@ $< $(CFLAGS)
endif

clean:
	rm *.o
	rm prodcomm
