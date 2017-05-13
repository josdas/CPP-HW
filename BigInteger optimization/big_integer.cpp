#include "big_integer.h"

#include <stdexcept>
#include <algorithm>

using namespace std;
typedef unsigned int digit_type;
typedef unsigned long long double_digit_type;
typedef long long sign_double_digit_type;

const sign_double_digit_type pow_digit = 32;
const sign_double_digit_type max_digit = 1ll << pow_digit;
const double_digit_type base_10 = 10;

template <typename T>
bool is_neg(T x) {
	return x < 0;
}

vector<double_digit_type> get_number_from_str_base10(string const& str) {
	vector<double_digit_type> temp;
	int end;
	if (str[0] == '-') {
		end = 1;
	}
	else {
		end = 0;
	}
	for (size_t ii = str.size(); ii > end; --ii) {
		size_t i = ii - 1;
		if (str[i] < '0' || str[i] > '9') {
			throw runtime_error("incorrect_char");
		}
		temp.push_back(str[i] - '0');
	}
	return temp;
}

digit_type get_next_digit_base10(vector<double_digit_type>& num, double_digit_type base) {
	if (base == 0) {
		throw runtime_error("base_equals_zero");
	}
	double_digit_type carry = 0;
	for (size_t ii = num.size(); ii != 0; --ii) {
		size_t i = ii - 1;
		double_digit_type cur = num[i] + carry * base_10;
		num[i] = cur / base;
		carry = cur % base;
	}
	while (num.size() > 0 && num.back() == 0) {
		num.pop_back();
	}
	return static_cast<digit_type>(carry);
}

big_integer::big_integer() {
	sign = false;
}

big_integer::big_integer(big_integer const& T) {
	data = T.data;
	sign = T.sign;
}

big_integer::big_integer(int x) {
	if (x == 0) {
		sign = false;
	}
	else {
		sign = is_neg(x);
		data = vector<digit_type>(1, x & (max_digit - 1));
		normalize();
	}
}

big_integer::big_integer(digit_type a) {
	data = vector<digit_type>(1, a);
	sign = false;
}

big_integer::big_integer(vector<digit_type> const& T, bool _sign) : sign(_sign), data(T) {
	normalize();
}

big_integer::big_integer(string const& str) {
	if (str.size() == 0) {
		throw runtime_error("empty_string");
	}
	bool isNeg = str[0] == '-';
	vector<double_digit_type> temp = get_number_from_str_base10(str);
	vector<digit_type> tempData;
	while (temp.size() > 0) {
		tempData.push_back(get_next_digit_base10(temp, max_digit));
	}
	sign = false;
	data = tempData;
	set_sign(isNeg);
	normalize();
}

big_integer& big_integer::operator=(big_integer const& other) {
	big_integer T(other);
	swap(T);
	return *this;
}

big_integer operator*(big_integer const& first, big_integer const& second) {
	bool sign = first.is_negative() ^ second.is_negative();
	big_integer abs_first(first.absolute_value());
	big_integer abs_second(second.absolute_value());
	if (abs_first.length() > abs_second.length()) {
		abs_first.swap(abs_second);
	}
	vector<digit_type> temp(abs_first.length() + abs_second.length() + 1);
	for (size_t i = 0; i < abs_first.length(); i++) {
		double_digit_type carry = 0;
		for (size_t j = 0; j < abs_second.length(); j++) {
			size_t k = i + j;
			double_digit_type tmp = static_cast<double_digit_type>(abs_first.get_digit(i)) * abs_second.get_digit(j);
			double_digit_type e = temp[k] + (tmp & (max_digit - 1)) + carry;
			temp[k] = e & max_digit - 1;
			carry = (tmp >> pow_digit) + (e >> pow_digit);
		}
		if (carry != 0) {
			temp[i + abs_second.length()] += static_cast<digit_type>(carry);
		}
	}
	big_integer result(temp, false);
	result.set_sign(sign);
	return result;
}

big_integer get_higher_digits(big_integer const& number, size_t n) {
	vector<digit_type> temp(n);
	for (size_t i = 0; i < temp.size(); i++) {
		if (i < number.length()) {
			temp[temp.size() - i - 1] = number.get_digit(number.length() - i - 1);
		}
	}
	return big_integer(temp, false);
}

double timed;

digit_type get_next_digit(big_integer const& first, big_integer const& second) {
	if (second == 0) {
		throw runtime_error("divition_by_zero");
	}
	big_integer abs_first = get_higher_digits(first, 4);
	big_integer abs_second = get_higher_digits(second, 3);
	if (abs_first < abs_second) {
		return 0;
	}
	abs_second = abs_second << pow_digit;
	for (digit_type i = 0; i <= 1; i++) {
		digit_type dig = 0;
		if (abs_first >= abs_second) {
			for (digit_type jj = pow_digit; jj != 0; jj--) {
				digit_type j = jj - 1;
				digit_type nw = dig + (1ll << j);
				if (nw * abs_second <= abs_first) {
					dig = nw;
				}
			}
			abs_first = abs_first - dig * abs_second;
		}
		if (dig > 0) {
			return dig;
		}
		abs_second = abs_second >> pow_digit;
	}
	return 0;
}

big_integer operator/(big_integer const& first, big_integer const& second) {
	if (second == 0) {
		throw runtime_error("divition_by_zero");
	}
	bool sign = first.is_negative() ^ second.is_negative();
	big_integer abs_first(first.absolute_value());
	big_integer abs_second(second.absolute_value());
	if (abs_first < abs_second) {
		return big_integer(0);
	}
	if (abs_second.length() == 1) {
		big_integer result = abs_first / abs_second.get_digit(0);
		if (sign) {
			result = -result;
		}
		return result;
	}
	size_t t = abs_first.length() - abs_second.length();
	if (abs_first < abs_second << static_cast<digit_type>(t) * pow_digit) {
		t--;
	}
	vector<digit_type> temp;
	for (size_t ii = t + 1; ii != 0; ii--) {
		size_t i = ii - 1;
		digit_type dig = get_next_digit(abs_first, abs_second);
		big_integer tmp = dig * abs_second;
		sign_double_digit_type carry = 0;
		for (size_t j = i; j < abs_first.length(); j++) {
			sign_double_digit_type res = carry + abs_first.data[j];
			if (j - i < tmp.length()) {
				res -= tmp.data[j - i];
			}
			if (res < 0) {
				carry = -1;
				res += max_digit;
			}
			else {
				carry = 0;
			}
			abs_first.data[j] = static_cast<digit_type>(res);
		}
		abs_first.normalize();
		temp.push_back(dig);
	}
	for (size_t i = 0; i < temp.size() / 2; i++) {
		swap(temp[i], temp[temp.size() - 1 - i]);
	}
	big_integer result(temp, false);
	result.set_sign(sign);
	return result;
}

big_integer operator%(big_integer const& first, big_integer const& second) {
	return first - (first / second) * second;
}

big_integer operator+(big_integer const& first, big_integer const& second) {
	double_digit_type carr = 0;
	size_t size = max(first.length(), second.length()) + 2;
	vector<digit_type> temp(size);
	for (size_t i = 0; i < size; i++) {
		double_digit_type val = carr + first.get_inf_digit(i) + second.get_inf_digit(i);
		carr = val >> pow_digit;
		temp[i] = val & (max_digit - 1);
	}
	bool sgn = (temp[size - 1] >> (pow_digit - 1)) > 0;
	return big_integer(temp, sgn);
}

bool operator==(big_integer const& a, big_integer const& b) {
	return a.data == b.data && a.sign == b.sign;
}

bool operator!=(big_integer const& a, big_integer const& b) {
	return !(a == b);
}

bool operator<(big_integer const& first, big_integer const& second) {
	if (first.get_sign() != second.get_sign()) {
		return first.get_sign() < second.get_sign();
	}
	if (first.length() != second.length()) {
		return first.length() < second.length();
	}
	for (size_t ii = first.length(); ii != 0; ii--) {
		size_t i = ii - 1;
		if (first.get_digit(i) != second.get_digit(i)) {
			return first.get_digit(i) < second.get_digit(i);
		}
	}
	return false;
}

bool operator>(big_integer const& a, big_integer const& b) {
	return b < a;
}

bool operator<=(big_integer const& a, big_integer const& b) {
	return !(a > b);
}

bool operator>=(big_integer const& a, big_integer const& b) {
	return !(a < b);
}

string to_string(big_integer const& arg) {
	string result;
	big_integer abs_arg = arg.absolute_value();
	bool isNeg = arg.is_negative();
	while (abs_arg.length() > 0) {
		const int block = 1000 * 1000 * 1000;
		const int pow_10_block = 9;
		int t = abs_arg % block;
		abs_arg = abs_arg / block;
		for (int i = 0; i < pow_10_block; i++) {
			result.push_back('0' + t % 10);
			t /= 10;
		}
	}
	while (result.size() > 1 && result.back() == '0') {
		result.pop_back();
	}
	if (result.empty()) {
		result.push_back('0');
	}
	if (isNeg) {
		result.push_back('-');
	}
	reverse(result.begin(), result.end());
	return result;
}

template <typename E>
big_integer div_big_small(big_integer const& first, E second) {
	if (second == 0) {
		throw runtime_error("divition_by_zero");
	}
	big_integer abs_first(first.absolute_value());
	bool isNeg = first.is_negative() ^ is_neg(second);
	double_digit_type div = abs(static_cast<sign_double_digit_type>(second));
	size_t size = first.length();
	vector<digit_type> temp(size);
	double_digit_type carry = 0;
	for (size_t ii = size; ii != 0; ii--) {
		size_t i = ii - 1;
		double_digit_type cur = abs_first.get_digit(i) + carry * max_digit;
		temp[i] = static_cast<digit_type>(cur / div);
		carry = cur % div;
	}
	big_integer T(temp, false);
	T.set_sign(isNeg);
	return T;
}

big_integer operator/(big_integer const& first, int second) {
	return div_big_small<int>(first, second);
}

big_integer operator/(big_integer const& first, digit_type second) {
	return div_big_small<digit_type>(first, second);
}

int operator%(big_integer const& first, int second) {
	if (second == 0) {
		throw runtime_error("mod_by_zero");
	}
	big_integer abs_first(first.absolute_value());
	size_t size = first.length();
	int carry = 0;
	for (size_t ii = size; ii != 0; ii--) {
		size_t i = ii - 1;
		double_digit_type cur = abs_first.get_digit(i) + carry * max_digit;
		carry = static_cast<int>(cur % second);
	}
	return carry;
}

big_integer operator-(big_integer const& first) {
	return (~first) + 1;
}

big_integer operator-(big_integer const& first, big_integer const& second) {
	return first + (-second);
}

big_integer operator+(big_integer const& first) {
	return big_integer(first);
}

big_integer operator++(big_integer& first) {
	big_integer temp = first + 1;
	first.swap(temp);
	return temp;
}

big_integer operator~(big_integer const& first) {
	size_t size = first.length();
	vector<digit_type> temp(size);
	for (size_t i = 0; i < size; i++) {
		temp[i] = ~first.get_inf_digit(i) & static_cast<digit_type>(max_digit - 1);
	}
	return big_integer(temp, !first.is_negative());
}

enum type_bit_operation {
	xor_,
	or_,
	and_
};

template <typename T>
T do_bit_operation(T a, T b, type_bit_operation type) {
	switch (type) {
	case xor_:
		return a ^ b;
	case or_:
		return a | b;
	case and_:
		return a & b;
	}
	return 0;
}

big_integer bit_operation(big_integer const& first, big_integer const& second, type_bit_operation type) {
	size_t size = max(first.length(), second.length());
	vector<digit_type> temp(size);
	for (size_t i = 0; i < size; i++) {
		temp[i] = do_bit_operation(first.get_inf_digit(i), second.get_inf_digit(i), type);
	}
	return big_integer(temp, do_bit_operation(first.is_negative(), second.is_negative(), type));
}

big_integer operator&(big_integer const& first, big_integer const& second) {
	return bit_operation(first, second, and_);
}

big_integer operator|(big_integer const& first, big_integer const& second) {
	return bit_operation(first, second, or_);
}

big_integer operator^(big_integer const& first, big_integer const& second) {
	return bit_operation(first, second, xor_);
}

big_integer operator>>(big_integer const& first, digit_type shift) {
	if (shift == 0) {
		return big_integer(first);
	}
	double_digit_type mod = shift % pow_digit;
	double_digit_type div = shift / pow_digit;
	size_t size = div < first.length() ? first.length() - div : 0;
	vector<digit_type> temp(size);
	for (size_t i = 0; i < size; i++) {
		temp[i] = (static_cast<double_digit_type>(first.get_inf_digit(i + div)) >> mod)
			| (static_cast<double_digit_type>(first.get_inf_digit(i + div + 1)) << (pow_digit - mod));
	}
	return big_integer(temp, first.is_negative());
}

big_integer operator<<(big_integer const& first, digit_type shift) {
	if (shift == 0) {
		return big_integer(first);
	}
	double_digit_type mod = shift % pow_digit;
	double_digit_type div = shift / pow_digit;
	size_t size = first.length() + div + 1;
	vector<digit_type> temp(size);
	temp[div] = (first.get_inf_digit(0) << mod);
	for (size_t i = div; i < size; i++) {
		temp[i] = (first.get_inf_digit(i - div) << mod)
			| static_cast<double_digit_type>(first.get_inf_digit(i - div - 1)) >> (pow_digit - mod);
	}
	return big_integer(temp, first.is_negative());
}

big_integer& big_integer::operator&=(big_integer const& rhs) {
	big_integer T = *this & rhs;
	swap(T);
	return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs) {
	big_integer T = *this | rhs;
	swap(T);
	return *this;
}

big_integer& big_integer::operator^=(big_integer const& rhs) {
	big_integer T = *this ^ rhs;
	swap(T);
	return *this;
}

void big_integer::swap(big_integer& t) noexcept {
	using std::swap;
	swap(sign, t.sign);
	swap(data, t.data);
}

size_t big_integer::length() const {
	return data.size();
}

int big_integer::get_sign() const {
	return sign ? -1 : 1;
}

bool big_integer::is_negative() const {
	return sign;
}

digit_type big_integer::get_digit(size_t i) const {
	return data[i];
}

digit_type big_integer::get_inf_digit(size_t i) const {
	if (i < length()) {
		return get_digit(i);
	}
	if (get_sign() == -1) {
		return max_digit - 1;
	}
	return 0;
}

big_integer big_integer::absolute_value() const {
	return is_negative() ? -*this : *this;
}

void big_integer::set_sign(bool s) {
	if (s != is_negative()) {
		*this = -*this;
	}
}

void big_integer::normalize() {
	while (data.size() > 0
		&& (get_sign() == -1 && data.back() == max_digit - 1
			|| get_sign() == 1 && data.back() == 0)) {
		data.pop_back();
	}
}

big_integer& big_integer::operator<<=(int rhs) {
	big_integer T = *this << rhs;
	swap(T);
	return *this;
}

big_integer& big_integer::operator>>=(int rhs) {
	big_integer T = *this >> rhs;
	swap(T);
	return *this;
}

big_integer& big_integer::operator+=(big_integer const& rhs) {
	big_integer T = *this + rhs;
	swap(T);
	return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs) {
	big_integer T = *this - rhs;
	swap(T);
	return *this;
}

big_integer& big_integer::operator*=(big_integer const& rhs) {
	big_integer T = *this * rhs;
	swap(T);
	return *this;
}

big_integer& big_integer::operator/=(big_integer const& rhs) {
	big_integer T = *this / rhs;
	swap(T);
	return *this;
}

big_integer& big_integer::operator%=(big_integer const& rhs) {
	big_integer T = *this % rhs;
	swap(T);
	return *this;
}

big_integer& big_integer::operator/=(int rhs) {
	big_integer T = *this / rhs;
	swap(T);
	return *this;
}

big_integer& big_integer::operator%=(int rhs) {
	big_integer T = *this % rhs;
	swap(T);
	return *this;
}
