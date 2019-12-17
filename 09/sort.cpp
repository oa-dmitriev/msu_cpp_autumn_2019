#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <sstream>
#include <algorithm>
#include "pool.h"

template <class T>
void print(const std::vector<T>& v) {
	for (auto& i : v) {
		std::cout << i << " ";
	}
	std::cout << '\n';
}

void sortAndWrite(std::vector<uint32_t>& v, size_t i) {
	sort(v.begin(), v.end());
	std::ostringstream os;
	os << "run" << i << ".dat";
	std::ofstream out(os.str(), std::ios::binary | std::ios::out);
	if (!out) {
		std::cerr << "can't open file" << std::endl;
		exit(1);
	}
	out.write(reinterpret_cast<char*>(v.data()), v.size() * sizeof(uint32_t));
	out.close();
}

template <class T>
void merge(std::vector<T>& v, const std::vector<T>& v0, const std::vector<T>& v1) {
	auto it0 = v0.begin();
	auto it1 = v1.begin();
	auto it = v.begin();

	while (it0 != v0.end() && it1 != v1.end()) {
		if (*it0 < *it1) {
			*it = *it0;
			it0++;
		} else {
			*it = *it1;
			it1++;
		}
		it++;
	}

	while (it0 != v0.end()) {
		*it = *it0;
		it0++;
		it++;
	}

	while (it1 != v1.end()) {
		*it = *it1;
		it1++;
		it++;
	}
}

void mergePair(const char* fn0, const char* fn1, size_t i) {
	using type = uint32_t;
	const size_t numThreads = 2;
	const size_t SIZE = 8 * 1024 * 1024 / sizeof(type) / numThreads / 2;
	std::ostringstream os;
	os << "run" << i << ".dat";
	std::ifstream ifs0(fn0, std::ios::binary | std::ios::in | std::ios::ate);
	std::ifstream ifs1(fn1, std::ios::binary | std::ios::in | std::ios::ate);
	std::ofstream out("merged.dat", std::ios::binary | std::ios::out);
	if (!ifs0) {
		std::cerr << "can't open file " << fn0 << std::endl;
		exit(1);	
	}
	if (!ifs1) {
		rename(fn0, os.str().c_str());	
		return;
	}
	std::streampos f0Size = ifs0.tellg();
	ifs0.seekg(0, std::ios::beg);
	std::streampos cur0 = ifs0.tellg();
	
	std::streampos f1Size = ifs1.tellg();
	ifs1.seekg(0, std::ios::beg);
	std::streampos cur1 = ifs1.tellg();

	std::vector<type> v0;
	std::vector<type> v1;
	std::vector<type> v;
	auto d0 = f0Size - ifs0.tellg();
	auto d1 = f1Size - ifs1.tellg();
	while (d0 > 0 && d1 > 0) {
		size_t numsLeft0 = d0 / sizeof(type);
		size_t numsLeft1 = d1 / sizeof(type);
		size_t size0 = SIZE, size1 = SIZE;
		if (numsLeft0 < SIZE) {
			size0 = numsLeft0;
			v0.resize(numsLeft0);
		} 
		if (numsLeft1 < SIZE) {
			size1 = numsLeft1;
			v1.resize(numsLeft1);
		}	
		ifs0.read(reinterpret_cast<char*>(v0.data()), size0 * sizeof(type));
		ifs1.read(reinterpret_cast<char*>(v1.data()), size1 * sizeof(type));
		v.resize(size0 + size1);
		merge<type>(v, v0, v1);
		out.write(reinterpret_cast<char*>(v.data()), v.size() * sizeof(type));
		d0 = f0Size - ifs0.tellg();
		d1 = f1Size - ifs1.tellg();
	}
	ifs0.close();
	ifs1.close();
	remove(fn0);
	remove(fn1);
	out.close();
	rename("merged.dat", os.str().c_str());
}

void mergeAllFiles(size_t numsRun) {
	if (numsRun == 1) {
		const char* oldName = "run0.dat";
		const char* newName = "result.dat";
		rename(oldName, newName);
		return;
	}
	size_t newRun;
	ThreadPool tp(2);
	for (size_t i = 0; i < numsRun; i += 2) {
		newRun = i / 2;
		std::ostringstream os0, os1;
		os0 << "run" << i << ".dat";
		os1 << "run" << i + 1 << ".dat";
		auto task = tp.exec(mergePair, os0.str().c_str(), os1.str().c_str(), newRun);
		task.get();
	}
	mergeAllFiles(newRun + 1);
}


int main() {
	using type = uint32_t;
	size_t SIZE = 8 * 1024 * 1024 / sizeof(type);
	std::ifstream ifs("data.dat", std::ios::binary | std::ios::in | std::ios::ate);
	std::streampos fsize = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	std::streampos cur = ifs.tellg();	
	std::vector<type> v;
	size_t numsRun;

	ThreadPool tp(2);
	for (numsRun = 0; fsize - cur > 0; ++numsRun) {
		size_t bytesLeft = fsize - cur;
		size_t numsLeft = bytesLeft / sizeof(type);
		if (numsLeft < SIZE) {
			v.resize(numsLeft);
			ifs.read(reinterpret_cast<char*>(v.data()), bytesLeft);
			auto task = tp.exec(sortAndWrite, std::ref(v), numsRun);
			task.get();
			numsRun++;
			break;	
		}
		v.resize(SIZE);
		ifs.read(reinterpret_cast<char*>(v.data()), SIZE * sizeof(type));
		auto task = tp.exec(sortAndWrite, std::ref(v), numsRun);
		task.get();
		cur = ifs.tellg();
	}
	ifs.close();
	mergeAllFiles(numsRun);
	return 0;
}
