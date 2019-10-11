#include <chrono>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <climits>

class Timer {
	using clock_t = std::chrono::high_resolution_clock;
	using microseconds = std::chrono::microseconds;
public:
	Timer()
		: start_(clock_t::now()) {}
	~Timer() {
		const auto finish = clock_t::now();
		const auto us =
			std::chrono::duration_cast<microseconds>
				(finish - start_).count();
		std::cout << us << " us" << std::endl;	
	}

private:
	const clock_t::time_point start_;
};

const size_t size = 10000;

int main() {
	int* a = new int[size * size];
	srand(time(0));	
	for (int i = 0; i < size * size; ++i) {
		a[i] = rand() % 255;
	}

	{
		Timer t;
		long long sum = 0;
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				sum += (int) a[i * size + j];
			}
		}
	}
	
	return 0;
}
