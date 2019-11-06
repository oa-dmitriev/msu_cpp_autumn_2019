CC=g++ -std=c++14

all: big_int

big_int: big_int.o
	$(CC) -o big_int big_int.o -O2

big_int.o: big_int.cpp
	$(CC) -c big_int.cpp

test: big_int
	./big_int

clean:
	rm -rf *.o big_int

