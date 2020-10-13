# See README for notes about project organization

CFLAGS=-std=c99 -Wall -pedantic

.PHONY:clean test all

all: prodcomm

prodcomm: main.o queue.o workers.o stat.o
	scan-build gcc -o prodcomm main.o queue.o workers.o stat.o -pthread

main.o: main.c workers.h queue.h stat.h
ifeq ($(DEBUG),true)
	gcc -g -c -o $@ $< $(CFLAGS)
else
	gcc -c -o $@ $< $(CFLAGS)
endif

queue.o: queue.c queue.h stat.h
ifeq ($(DEBUG),true)
	gcc -g -c -o $@ $< $(CFLAGS)
else
	gcc -c -o $@ $< $(CFLAGS)
endif

workers.o: workers.c workers.h queue.h stat.h
ifeq ($(DEBUG),true)
	gcc -g -c -o $@ $< $(CFLAGS)
else
	gcc -c -o $@ $< $(CFLAGS)
endif

stat.o: stat.c stat.h
ifeq ($(DEBUG),true)
	gcc -g -c -o $@ $< $(CFLAGS)
else
	gcc -c -o $@ $< $(CFLAGS)
endif

test: all
	@bash test.sh

clean:
	rm -f *.o
	rm -f prodcomm
	rm -f test.log
