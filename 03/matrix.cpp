#include <iostream>
#include <stdexcept>
#include "matrix.h"

Row::Row(size_t row, size_t col, int* arr)
		: row(row), col(col), arr(arr) {
}

int& Row::operator [](size_t j) {
	if (j >= col) {
		throw std::out_of_range("");
	}
	return arr[row * col + j];
}

const int Row::operator [](size_t j) const {
	if (j >= col) {
		throw std::out_of_range("");
	}
	return arr[row * col + j];
}

Matrix::Matrix(size_t row, size_t col)
		: row(row), col(col), arr(new int[row * col]) {}

Row Matrix::operator [](size_t i) {
	if (i >= row) {
		throw std::out_of_range("");
	}
	Row r(i, col, arr);
	return r;
}

const Row Matrix::operator [](size_t i) const {
	if (i >= row) {
		throw std::out_of_range("");
	}
	Row r(i, col, arr);
	return r;
}

const Matrix& Matrix::operator *=(int val) {
	for (size_t i = 0; i < row; ++i) {
		for (size_t j = 0; j < col; ++j) {
			arr[i * col + j] = arr[i * col + j] * val;	
		}
	}
	return *this;
}

bool Matrix::operator ==(const Matrix& other) const {
	if (this == &other) {
		return true;
	}
	if (row != other.row || col != other.col) {
		return false;
	}
	for (size_t i = 0; i < row; ++i) {
		for (size_t j = 0; j < col; ++j) {
			size_t ind = i * col + j;
			if (arr[ind] != other.arr[ind]) {
				return false;
			}
		}
	}
	return true;
}

bool Matrix::operator !=(const Matrix& other) const {
	return !(*this == other);
}

size_t Matrix::getRows() { return row; }
size_t Matrix::getColumns() { return col; }

Matrix::~Matrix() {
	delete[] arr;
}
