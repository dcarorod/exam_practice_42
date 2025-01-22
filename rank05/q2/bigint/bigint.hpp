#pragma once
#include <iostream>
#include <string>
#include <algorithm>

class bigint {
private:
	std::string _n;

public:
	bigint() : _n("0") {}
	bigint(size_t num) : _n(std::to_string(num)) {}
	bigint(std::string s) {
		if (s.empty() || !std::all_of(s.begin(), s.end(), ::isdigit)) {
			std::cerr << "Not a valid numerical string" << std::endl;
			_n = "0";
		}
		else
			_n = s;
	}
	bigint(const bigint& other) : _n(other._n) {}
	~bigint() {}

	std::string add_strings(const std::string s1, const std::string s2) const {
		std::string	res;
		size_t		idx1 = s1.size(), idx2 = s2.size(), num = 0;
		while (idx1 || idx2)
		{
			if (idx1) {--idx1;}
			if (idx2) {--idx2;}
			num = num + (s1[idx1] - '0') + (s2[idx2] - '0');
			if (num > 9) {
				res.push_back(num - 10 + '0');
				num = 10;
			}
			else {
				res.push_back(num + '0');
				num = 0;
			}
		}
		std::reverse(res.begin(), res.end());
		return res;
	}

	bigint operator+(const bigint& other) const {
		return bigint(add_strings(_n, other._n));
	}

	bigint operator+=(const bigint& other) {
		_n = add_strings(_n, other._n);
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& os, const bigint& rh) {
		return os << rh._n;
	}
};