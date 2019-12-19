#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cassert>

int main() {
	using type = uint32_t;
	std::ifstream ifs("result.dat", std::ios::binary | std::ios::in | std::ios::ate);
	if (!ifs) {
		assert("file not found" == "");
	}
	std::streampos fSize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::vector<type> v(fSize / sizeof(type));
	ifs.read(reinterpret_cast<char*>(v.data()), fSize);
	assert(std::is_sorted(std::begin(v), std::end(v)) == 1);	
	return 0;
}
