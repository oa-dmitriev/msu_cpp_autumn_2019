#pragma once

class LinearAllocator {
private:
	size_t maxSize;
	char* arr;
	char* start;
	char* end;
public:
	LinearAllocator(size_t maxSize);
	~LinearAllocator();
	char* alloc(size_t size);
	void reset();
};
