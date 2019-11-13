CC=g++ -std=c++14

all: serializer

serializer: serializer.o
	$(CC) -o serializer test.o -O2

serializer.o: test.cpp serializer.h
	$(CC) -c test.cpp

test: serializer
	./serializer

clean:
	rm -rf *.o serializer
