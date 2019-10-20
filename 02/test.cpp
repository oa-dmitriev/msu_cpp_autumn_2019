#include <iostream>
#include <chrono>
#include "time.h"
#include "allocator.h"

#define check(cond) do { if (!(cond)) std::cout << "line " << __LINE__ << ": " << #cond << '\n'; } while(0)

int main() {
	Timer t;
	LinearAllocator a(10);
	char* p;

	p = a.alloc(11);
	check(p == nullptr);

	p = a.alloc(5);
	check(p != nullptr);

	p = a.alloc(5);
	check(p != nullptr);	
	
	p = a.alloc(1);
	check(p == nullptr);
	
	a.reset();
	p = a.alloc(1);
	check(p != nullptr);
	return 0;
}
