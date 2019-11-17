CC=g++ -std=c++14

all: format

format: format.o test.o
	$(CC) -o test test.o format.o -O2

format.o: format.cpp format.h
	$(CC) -c format.cpp

test.o: test.cpp format.h
	$(CC) -c test.cpp

test: format
	./test

clean:
	rm -rf *.o format test