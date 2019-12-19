CC=g++ -std=c++11

all: sort


gen: genRandNumbers.cpp
	$(CC) genRandNumbers.cpp -o gen
	./gen

sort: sort.o
	$(CC) -o sort sort.o -lpthread

sort.o: sort.cpp pool.h
	$(CC) -c sort.cpp -lpthread

run: sort
	./sort

test: gen run test.cpp
	$(CC) test.cpp -o test
	./test

clean:
	rm -rf *.o sort *.dat *.log *.out gen test
