#include <chrono>
#include <iostream>
#include "time.h"

Timer::Timer()
	: start_(std::chrono::high_resolution_clock::now()) {}

Timer::~Timer() {
	using clock_t = std::chrono::high_resolution_clock;
	using microseconds = std::chrono::microseconds;
	const auto finish = clock_t::now();
	const auto us =
		std::chrono::duration_cast<microseconds>
			(finish - start_).count();
	std::cout << us << " us" << std::endl;
}
