CC=g++ -std=c++11

all: pool

pool: pool.o
	$(CC) -o pool pool.o -O2 -lpthread

pool.o: pool.cpp
	$(CC) -c pool.cpp

run: pool
	./pool
	
clean:
	rm -rf *.o pool