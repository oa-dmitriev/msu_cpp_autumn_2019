CC=g++ -std=c++11

all: sort

sort: sort.o
	$(CC) -o sort sort.o -lpthread

sort.o: sort.cpp pool.h
	$(CC) -c sort.cpp -lpthread

run: sort
	./sort

clean:
	rm -rf *.o sort result.dat
