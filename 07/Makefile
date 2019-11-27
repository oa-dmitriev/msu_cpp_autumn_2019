CC=g++ -std=c++14

all: container

container: container.o
	$(CC) -o container container.o -O3

container.o: container.cpp
	$(CC) -c container.cpp

run: container
	./container

test: container
	./container
	
clean: 
	rm -rf *.o container