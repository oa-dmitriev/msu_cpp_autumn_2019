#include <iostream>
#include <cassert>
#include "matrix.h"

int main() {

	
	Matrix m(1, 1);
	m[0][0] = 100;
	
	const Matrix& m0 = m;
	m[0][0] = m0[0][0] + 4;
	assert(m[0][0] == 104);

	Matrix m1(1, 3);
	for (int i = 0; i < 3; ++i) {
		m1[0][i] = i;
	}
	assert(m1[0][0] == 0);
	assert(m1[0][1] == 1);
	assert(m1[0][2] == 2);
	
	assert(m1.getRows() == 1);
	assert(m1.getColumns() == 3);
	
	try {
		int val = m1[0][3];
	} catch (std::out_of_range&) {
	
	} catch (...) {
		std::cout << "line " << __LINE__ << std::endl;
	}

		
	const Matrix& m2 = m1;
	m1 *= 2;
	assert(m2[0][0] == 0);
	assert(m2[0][1] == 2);
	assert(m2[0][2] == 4);
	
	Matrix m3(0, 0);
	assert(m1 == m1);
	assert(m1 != m3);
	return 0;
}
