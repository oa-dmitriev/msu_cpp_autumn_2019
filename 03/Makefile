CC=g++ -std=c++14

all: matrix

matrix: matrix.o test.o
	$(CC) -o test matrix.o test.o -O2
matrix.o: matrix.cpp matrix.h
	$(CC) -c matrix.cpp
test.o:  test.cpp matrix.h
	$(CC) -c test.cpp
test: matrix
	./test
clean:
	rm -rf *.o test
