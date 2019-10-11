#include <iostream>
#include <vector>
#include <algorithm>

bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) 
		++it;
    return !s.empty() && it == s.end();
}

double next_number(std::vector<std::string> &v, size_t &pos) {
	int sign = 1;
	double num;
	if (v[pos] == "-") {
		sign = -1;
		pos++;
	}
	if (pos < v.size() && is_number(v[pos])) {
		return sign * atof(v[pos].c_str());
	}
	throw 0;
}

double mul(std::vector<std::string> &v, size_t &pos) {
	if (pos >= v.size()) {
		throw 0;
	}

	double res = next_number(v, pos);
	++pos;
	if (pos < v.size()) {
		std::string next = v[pos];
		if (next == "*") {
			res *= mul(v, ++pos);
		}
		if (next == "/") {
			res /= mul(v, ++pos);
		}
	}
	return res;
}

double sum(std::vector<std::string> &v, size_t &pos) {
	double res = mul(v, pos);
	while (pos < v.size()) {
		if (v[pos] == "+") {
			res += mul(v, ++pos);
		} else 	if (v[pos] == "-") {
			res -= mul(v, ++pos);
		}
	}
	return res;
}

bool is_op(char c) {
	return c == '+' || c == '-' || c == '*' || c == '/';
}

std::vector<std::string> parse(std::string &s) {
	s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
	std::vector<std::string> v;
	for (int i = 0; i < s.size(); ++i) {
		std::string token;
		if (isdigit(s[i])) {
			while(isdigit(s[i])) {
				token.append(1, s[i]);
				i++;
			}
			i--;
		} else if (is_op(s[i])) {
			token.append(1, s[i]);
		} else {
			throw 0;
		}
		v.push_back(token);
	}
	return v;
}

void print(std::vector<std::string> &v) {
	for (int i = 0; i < v.size(); ++i) {
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char* argv[]) {
	std::string expr;
	if (argc == 2) {
		expr = std::string(argv[1]);
		std::vector<std::string> v;
		try {
			v = parse(expr);
			double result;
			size_t pos = 0;
			result = sum(v, pos);
			std::cout << result;
		} catch (...) {
			//std::cerr << "Wrong input format" << std::endl;
		}
	} else {
		//std::cerr << "Wrong input format" << std::endl;
	}
	return 0;
}
