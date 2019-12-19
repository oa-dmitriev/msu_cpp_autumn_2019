#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>

int main() {
	using type = uint32_t;
	srand(time(NULL));
	std::ofstream file("data.dat", std::ios::binary | std::ios::out);	
	for (size_t i = 0; i < 1000; ++i) {
		type num = rand() % 100;
		file.write(reinterpret_cast<const char*>(&num), sizeof(type));
	}	
	return 0;
}
