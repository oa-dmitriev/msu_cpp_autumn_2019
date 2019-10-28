#pragma once

class Row {
public:
	Row(size_t row, size_t col, int* arr);
	int& operator [](size_t j);
	const int operator [](size_t j) const;
private:
	size_t row;
	size_t col;
	int* arr;
};

class Matrix {
public:
	Matrix(size_t row, size_t col);
	~Matrix();
	Row operator [](size_t i);
	const Row operator [](size_t i) const;
	Matrix& operator *=(int val); 
	bool operator ==(const Matrix& other) const;
	bool operator !=(const Matrix& other) const;
	size_t getRows();
	size_t getColumns();
private:
	size_t row;
	size_t col;
	int* arr;
};
