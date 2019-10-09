CC=g++ -std=c++14

all: sum_by_columns sum_by_rows

sum_by_rows: sum_by_rows.o
	$(CC) -o sum_by_rows sum_by_rows.o -O2

sum_by_rows.o: sum_by_rows.cpp
	$(CC) -c sum_by_rows.cpp

sum_by_columns: sum_by_columns.o
	$(CC) -o sum_by_columns sum_by_columns.o -O2

sum_by_columns.o: sum_by_columns.cpp
	$(CC) -c sum_by_columns.cpp

run:
	./sum_by_columns
	./sum_by_rows

clean:
	rm -rf *.o sum_by_rows sum_by_columns
