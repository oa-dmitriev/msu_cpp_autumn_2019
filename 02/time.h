#pragma once

class Timer {
private:
	const std::chrono::high_resolution_clock::time_point start_;
public:
	Timer();
	~Timer();
};
