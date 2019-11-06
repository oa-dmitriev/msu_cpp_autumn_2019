#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <sstream>
#include <assert.h>

const int base = 1000000000;
class BigInt {
public:
	BigInt(int* arr, size_t size, int sign) : 
			arr_(arr), size_(size), sign_(sign) {}

	BigInt() {
		arr_ = new int[1];
		arr_[0] = 0;
		size_ = 1;
		sign_ = 0;
	}

	BigInt(int a) {
		if (a < 0) {
			sign_ = 1;
			a = -a;
		} else {
			sign_ = 0;
		}
		arr_ = new int[1];
		arr_[0] = a;
		size_ = 1;
	}

	BigInt(const std::string& str) {
		std::string s = str;
		if (s[0] == '-') {
			sign_ = 1;
			s = s.substr(1, s.length() - 1);
		} else {
			sign_ = 0;
		}
		if (s.length() % 9 == 0) {
			size_ = s.length() / 9;
		} else {
			size_ = s.length() / 9 + 1;
		}
		arr_ = new int[size_];
		size_t j = 0;
		for (int i = s.length(); i > 0; i -= 9, j++) {
			if (i < 9) {
				arr_[j] = atoi(s.substr(0, i).c_str());
			} else {
				arr_[j] = atoi(s.substr(i - 9, 9).c_str());
			}
		}
	}

	bool compare(int* a, size_t sza, int* b, size_t szb) const {
		if (sza > szb) {
			return true;
		} else if (sza < szb) {
			return false;
		} else {
			for (int i = size_; i >= 0; i--) {
				if (a[i] > b[i]) {
					return true;
				}
				if (a[i] < b[i]) {
					return false;
				}
			}
		}
		return false;
	}

	bool operator>(const BigInt& other) const {
		if (sign_ < other.getSign()) {
			return true;
		} else if (sign_ > other.getSign()) {
			return false;
		} else if (sign_ == 0) {
			return compare(arr_, size_, other.getNum(), other.getSize());
		} else {
			return !compare(arr_, size_, other.getNum(), size_);
		}
	}

	bool operator==(const BigInt& other) const {
		if (this == &other) {
			return true;
		}
		if (arr_[size_ - 1] == 0 && 
					other.getNum()[other.getSize() - 1] == 0) {
			return true;
		}
		if (sign_ == other.getSign()) {
			return !(compare(arr_, size_, other.getNum(), other.getSize()) || 
					compare(other.getNum(), other.getSize(), arr_, size_));
		}
		return false;
	}

	bool operator<(const BigInt& other) const {
		if (this == &other) {
			return false;
		}
		return !(*this > other || *this == other);
	}

	int* get_sum(int* a, size_t sza, int* b, size_t szb, size_t& size) const {
		size_t sz = sza > szb ? sza : szb;
		int* arr = new int[sz + 1];
		int* small;
		size_t ss;
		if (sza >= szb) {
			memcpy(arr, a, sza * sizeof(int));
			ss = szb;
			small = b;
		} else {
			memcpy(arr, b, szb * sizeof(int));
			ss = sza;
			small = a;
		}
		int carry = 0;
		size_t i;
		for (i = 0; i < ss; ++i) {
			arr[i] = arr[i] + small[i] + carry;
			carry = arr[i] / base;
			arr[i] %= base;
		}
		while (carry) {
			arr[i] += carry;
			carry = arr[i] / base;
			arr[i] %= base;
			i++;
		}
		size = sz > i ? sz : i;
		return arr;
	}

	int* get_subtr(int* a, size_t sza, int* b, size_t szb, size_t& size) const {
		int carry = 0;
		int* arr = new int[sza];
		memcpy(arr, a, sza * sizeof(int));
		size_t i;
		for (i = 0; i < szb; ++i) {
			arr[i] = arr[i] - b[i] - carry;
			if (arr[i] < 0) {
				arr[i] += base;
				carry = 1;
			} else {
				carry = 0;
			}
		}
		while (carry) {
			arr[i] = arr[i] - carry;
			if (arr[i] < 0) {
				arr[i] += base;
				carry = 1;
			} else {
				carry = 0;
			}
			i++;
		}
		i = sza - 1;
		while (i > 0 && arr[i] == 0) {
			i--;
		}
		size = i + 1;
		return arr;
	}

	BigInt operator+(const BigInt& other) const {
		int* arr;
		size_t sz = 0;
		int sign = sign_;
		if (!(sign_ || other.getSign())) {
			arr = get_sum(arr_, size_, other.getNum(), other.getSize(), sz);
		} else if (sign_ == other.getSign()) {
			arr = get_sum(arr_, size_, other.getNum(), other.getSize(), sz);
		} else if (compare(arr_, size_, other.getNum(), other.getSize())) {
			arr = get_subtr(arr_, size_, other.getNum(), other.getSize(), sz);
			sign = sign_;
		} else {
			arr = get_subtr(other.getNum(), other.getSize(), arr_, size_, sz);
			sign = other.getSign();
		}
		return BigInt(arr, sz, sign);
	}

	BigInt operator-(const BigInt& other) const {
		return -other + *this;
	}

	BigInt operator+(int a) const {
		return *this + BigInt(a);
	}

	BigInt operator-(int a) const {
		return *this - BigInt(a);
	}

	BigInt operator-() const {
		int* arr = new int[size_];
		memcpy(arr, arr_, size_ * sizeof(int));
		return BigInt(arr, size_, (sign_ + 1) % 2);
	}

	bool operator!=(const BigInt& other) const {
		return !(*this == other);
	}
	bool operator<=(const BigInt& other) const {
		return (*this < other || *this == other);
	}
	bool operator>=(const BigInt& other) const {
		return (*this > other || *this == other);
	}

	int* getNum() const {
		return arr_;
	}
	size_t getSize() const {
		return size_;
	}
	int getSign() const {
		return sign_;
	}
	int* arr_;
	int size_;
	int sign_;
};

std::ostream& operator<< (std::ostream& out, const BigInt& obj) {
	int* arr = obj.getNum();
	int size = obj.getSize();
	int sign = obj.getSign();
	if (sign && arr[size - 1] != 0) {
		out << "-";
	}
	out << arr[size - 1];
	for (int i = size - 2; i >= 0; i--) {
		std::stringstream ss;
		ss << arr[i];
		std::string s;
		ss >> s;
		size_t l = s.length();
		while (l < 9) {
			s = "0" + s;
			l++;
		}
		out << s;
	}
	return out;
}

std::string toString(const BigInt& value)
{
    std::stringstream buf;
    buf << value;
    return buf.str();
}

void check(int64_t x, int64_t y)
{
    const BigInt bigX = x;
    const BigInt bigY = y;

    if (bigX + bigY != BigInt(x + y))
    {
        std::cout << x << " + " << y << " != " << x + y << " got " << bigX + bigY << '\n';
    }

    if (bigX - bigY != BigInt(x - y))
    {
        std::cout << x << " - " << y << " != " << x - y << " got " << bigX - bigY << '\n';
    }
}

int main() {
	BigInt x = 10;
	assert(toString(x) == "10");
    BigInt y = x;
    assert(toString(y) == "10");
    BigInt z;
    assert(toString(z) == "0");
    assert(toString(BigInt(-10)) == "-10");
    assert(x == x);
    assert(y == x);
    assert(x != z);
    assert(y != z);

    assert(!(y > x));
    assert(!(x < y));
  	assert(x > z);
  	assert(y > z);
  	assert(x < 11);

  	assert(BigInt(-12) > BigInt(-123213));
  	assert(y >= x);
  	assert(y <= x);
  	assert(y >= BigInt(4));

   	assert(BigInt(0) == -BigInt(0));

   	assert(toString(BigInt(3) + BigInt(2)) == "5");
   	assert(toString(BigInt(2) + BigInt(-3)) == "-1");
   	assert(toString(BigInt(-2) + BigInt(-3)) == "-5");

   	assert(toString(BigInt(10) + BigInt(-10)) == "0");
   	assert(toString(-BigInt(10) + BigInt(10)) == "0");

   	assert(toString(BigInt(-1) + BigInt(1)) == "0");
    
    BigInt a("12318293745912834182634781263489123956");
    BigInt b = a + a;
    assert(toString(b) == "24636587491825668365269562526978247912");
    assert(toString(a - a) == "0");

    BigInt c("-12318293745912834182634781263489123956");
    assert(c + c == -b);
	return 0;
}