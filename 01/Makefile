CC=g++ -std=c++14

all: calc

calc: calc.o
	$(CC) -o calc calc.o -O2

calc.o: calc.cpp
	$(CC) -c calc.cpp

run: calc
	./calc

test: calc
	python3 test.py

clean:
	rm -rf *.o calc
