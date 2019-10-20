CC=g++ -std=c++14

all: allocator

allocator: allocator.o time.o test.o
	$(CC) -o test allocator.o time.o test.o -O2

allocator.o: allocator.h
	$(CC) -c allocator.cpp

time.o: time.cpp time.h
	$(CC) -c time.cpp

test.o: test.cpp time.h allocator.h
	$(CC) -c test.cpp

test: allocator
	./test

clean: 
	rm -rf *.o test
