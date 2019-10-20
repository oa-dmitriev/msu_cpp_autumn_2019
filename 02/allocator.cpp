#include <iostream>

#include "allocator.h"


LinearAllocator::LinearAllocator(size_t maxSize) :
					maxSize(maxSize),
					arr(new char[maxSize]),
					start(arr),
					end(start + maxSize) {}

LinearAllocator::~LinearAllocator() { delete[] arr; }

char* LinearAllocator::alloc(size_t size) {
	char* p = nullptr;
	if (end - start >= size) {
		p = start;
		start = start + size;
	}
	return p;
}

void LinearAllocator::reset() {
	start = arr;
}
