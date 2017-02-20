
#ifndef _FRAC_H_
#define _FRAC_H_

#include <stdexcept>
#include <cstdint>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>

template<typename T>
class Frac
{
	private:
		bool s;
		T n, d;
		static T gcd(T x, T y);
		inline void standarize();
	public:
		const T &num, &den;
		const bool &neg;
		Frac(T numerator = 0, T denominator = 1, bool negative = false) : s(negative), n(numerator), d(denominator), num(n), den(d), neg(s) {standarize();}
		Frac(const Frac &rhs) : s(rhs.s), n(rhs.n), d(rhs.d), num(n), den(d), neg(s) {}
		inline Frac & operator=(const Frac &rhs);
		operator double() const {return s ? - static_cast<double>(n) / static_cast<double>(d) : static_cast<double>(n) / static_cast<double>(d);}
		Frac operator-() const {return Frac(n, d, !neg);}
		Frac inverse() const {return Frac(d, n, neg);}
		inline Frac & operator+=(const Frac &rhs);
		inline Frac & operator-=(const Frac &rhs);
		inline Frac & operator*=(const Frac &rhs);
		inline Frac & operator/=(const Frac &rhs);
};

template<typename T>
T Frac<T>::gcd(T x, T y)
{
	if (y == 0)
		return x;
	return gcd(y, x % y);
}

template<typename T>
inline void Frac<T>::standarize()
{
	if (d == 0)
		throw std::invalid_argument("Denominator is 0");
	T c = gcd(n, d);
	n /= c;
	d /= c;
}

template<typename T>
inline Frac<T> & Frac<T>::operator=(const Frac<T> &rhs)
{
	s = rhs.s;
	n = rhs.n;
	d = rhs.d;
	return *this;
}

template<typename T>
Frac<T> operator+(const Frac<T> &lhs, const Frac<T> &rhs)
{
	if (lhs.neg)
		return -(-lhs + -rhs);
	if (rhs.neg)
		return lhs - -rhs;
	return Frac<T>(
			lhs.num * rhs.den + rhs.num * lhs.den,
			lhs.den * rhs.den,
			false);
}

template<typename T>
Frac<T> operator-(const Frac<T> &lhs, const Frac<T> &rhs)
{
	if (lhs.neg)
		return -(-lhs + rhs);
	if (rhs.neg)
		return lhs + -rhs;
	bool negative = lhs.num * rhs.den < rhs.num * lhs.den;
	return Frac<T>(
			negative ? rhs.num * lhs.den - lhs.num * rhs.den : lhs.num * rhs.den - rhs.num * lhs.den,
			lhs.den * rhs.den,
			negative);
}

template<typename T>
Frac<T> operator*(const Frac<T> &lhs, const Frac<T> &rhs)
{
	return Frac<T>(
			lhs.num * rhs.num,
			lhs.den * rhs.den,
			! lhs.neg == rhs.neg);
}

template<typename T>
Frac<T> operator/(const Frac<T> &lhs, const Frac<T> &rhs)
{
	return lhs * rhs.inverse();
}

template<typename T>
inline Frac<T> & Frac<T>::operator+=(const Frac<T> &rhs)
{
	return *this = *this + rhs;
}

template<typename T>
inline Frac<T> & Frac<T>::operator-=(const Frac<T> &rhs)
{
	return *this = *this - rhs;
}

template<typename T>
inline Frac<T> & Frac<T>::operator*=(const Frac<T> &rhs)
{
	return *this = *this * rhs;
}

template<typename T>
inline Frac<T> & Frac<T>::operator/=(const Frac<T> &rhs)
{
	return *this = *this / rhs;
}

template<typename Char, typename T>
inline std::basic_ostream<Char> & operator<<(std::basic_ostream<Char> &os, const Frac<T> &f)
{
	if (f.neg && f.num != 0)
		os << static_cast<Char>('-');
	os << f.num;
	if (f.den != 1)
		os << static_cast<Char>('/') << f.den;
	return os;
}

template<typename Char, typename T>
inline std::basic_istream<Char> & operator>>(std::basic_istream<Char> &is, Frac<T> &f)
{
	std::basic_string<Char> s;
	T num, den = 1;
	bool neg = false;
	is >> s;
	if (s[0] == static_cast<Char>('-'))
	{
		neg = true;
		s = s.substr(1);
	}
	size_t slash_pos = s.find_first_of(static_cast<Char>('/'));
	std::basic_istringstream<Char> iss;
	iss.str(s.substr(0, slash_pos));
	iss >> num;
	if (slash_pos != std::basic_string<Char>::npos)
	{
		iss.seekg(std::ios::beg);
		iss.str(s.substr(slash_pos + 1));
		iss >> den;
	}
	f = Frac<T>(num, den, neg);
	return is;
}

#endif
