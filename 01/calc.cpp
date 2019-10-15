#include <iostream>
#include <vector>
#include <algorithm>
#include <exception>
#include <cstring>

class WrongInput : public std::exception {
public:
	WrongInput(const std::string &msg, int line, int pos)
		: msg(msg), line(line), pos(pos) {}
	const char* what() const noexcept {
		return msg.c_str();
	}
	
	int getErrorLine() const throw() {
		return line;
	}
	int getPos() const throw() {
		return pos;
	}
private:
	std::string msg;
	int line;
	int pos;
};

inline bool is_number(const std::string& s) {
	return std::all_of(s.begin(), s.end(), isdigit);
}

double next_number(const std::vector<std::string> &v, size_t &pos) {
	int sign = 1;
	double num;
	if (v[pos] == "-") {
		sign = -1;
		pos++;
	}
	if (pos < v.size() && is_number(v[pos])) {
		return sign * atof(v[pos].c_str());
	}
	throw WrongInput("Expected number but got something else", __LINE__, pos);
}

double mul(const std::vector<std::string> &v, size_t &pos) {
	if (pos >= v.size()) {
		throw WrongInput("Expected number but got nothing", __LINE__, pos);
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

double sum(const std::vector<std::string> &v, size_t &pos) {
	double res = mul(v, pos);
	while (pos < v.size()) {
		if (v[pos] == "+") {
			res += mul(v, ++pos);
		} else 	if (v[pos] == "-") {
			res -= mul(v, ++pos);
		} else {
			throw WrongInput("Wrong symbol", __LINE__, pos);
		}
	}
	return res;
}

std::vector<std::string> parse(std::string &s) {
	s.erase(remove_if(s.begin(), s.end(), isspace), s.end());
	std::vector<std::string> v;
	for (int i = 0; i < s.size(); ++i) {
		std::string token = "";
		if (!isdigit(s[i])) {
			token.append(1, s[i]);
			v.push_back(token);
			continue;
		}

		while (isdigit(s[i])) {
			token.append(1, s[i]);
			i++;
		}
		i--;
		v.push_back(token);
	}
	return v;
}

void print(const std::vector<std::string> &v) {
	for (int i = 0; i < v.size(); ++i) {
		std::cout << v[i] << " ";
	}
	std::cout << std::endl;
}

int main(int argc, char* argv[]) {
	std::string expr;
	if (argc != 2) {
		std::cerr << "Plese enter a string as an argument" << expr << std::endl;
		return 1;
	}
	expr = std::string(argv[1]);
	std::vector<std::string> v;
	v = parse(expr);
	try {
		double result;
		size_t pos = 0;
		result = sum(v, pos);
		std::cout << result;
	} catch (WrongInput& ex) {
		std::cerr << "Error in line: " << ex.getErrorLine()
			  << ". " << ex.what() << " at position "
			  << ex.getPos() << " in the input expression: " 
			  << "\"" << expr << "\"" << std::endl;
		return 1;
	}
	return 0;
}
