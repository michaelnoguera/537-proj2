# See README for notes about project organization

CFLAGS=-std=c99 -Wall -pedantic
SCAN_BUILD_DIR=scan-build-out
TEST_RUNNER=test.sh

.PHONY:clean test all scan-build scan-view

all: prodcomm

# build executable
prodcomm: main.o queue.o workers.o stat.o
	gcc -o prodcomm main.o queue.o workers.o stat.o -pthread

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

# Run test framework
test: all
	@bash $(TEST_RUNNER)

# Clean files
clean:
	rm -f *.o
	rm -f prodcomm
	rm -f test.log
	rm -rf $(SCAN_BUILD_DIR)

# Run the Clang Static Analyzer
scan-build: clean
	scan-build -o $(SCAN_BUILD_DIR) make

# View the one scan avaialble
scan-view: scan-build
	xdg-open $(SCAN_BUILD_DIR)/*/index.html 
