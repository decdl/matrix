
#ifndef _FRAC_H_
#define _FRAC_H_

#include <cstdint>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>

template<typename T>
class frac
{
	private:
		bool s;
		T n, d;
		static T gcd(T x, T y);
		inline void standarize();
	public:
		const T &num, &den;
		const bool &neg;
		frac(T numerator = 0, bool negative = false, T denominator = 1) : s(negative), n(numerator), d(denominator), num(n), den(d), neg(s) {standarize();}
		frac(const frac<T> &rhs) : s(rhs.s), n(rhs.n), d(rhs.d), num(n), den(d), neg(s) {}
		inline frac<T> & operator=(const frac<T> &rhs);
		operator double() const {return s ? - static_cast<double>(n) / static_cast<double>(d) : static_cast<double>(n) / static_cast<double>(d);}
		frac<T> operator-() const {return frac<T>(n, !neg, d);}
		frac<T> inverse() const {return frac<T>(d, neg, n);}
		inline frac<T> & operator+=(const frac<T> &rhs);
		inline frac<T> & operator-=(const frac<T> &rhs);
		inline frac<T> & operator*=(const frac<T> &rhs);
		inline frac<T> & operator/=(const frac<T> &rhs);
};

template<typename T>
T frac<T>::gcd(T x, T y)
{
	if (y == 0)
		return x;
	return gcd(y, x % y);
}

template<typename T>
inline void frac<T>::standarize()
{
	if (d == 0)
		throw "Denominator is 0";
	T c = gcd(n, d);
	n /= c;
	d /= c;
}

template<typename T>
inline frac<T> & frac<T>::operator=(const frac<T> &rhs)
{
	s = rhs.s;
	n = rhs.n;
	d = rhs.d;
	return *this;
}

template<typename T>
frac<T> operator+(const frac<T> &lhs, const frac<T> &rhs)
{
	if (lhs.neg)
		return -(-lhs + -rhs);
	if (rhs.neg)
		return lhs - -rhs;
	return frac<T>(
			lhs.num * rhs.den + rhs.num * lhs.den, false,
			lhs.den * rhs.den);
}

template<typename T>
frac<T> operator-(const frac<T> &lhs, const frac<T> &rhs)
{
	if (lhs.neg)
		return -(-lhs + rhs);
	if (rhs.neg)
		return lhs + -rhs;
	bool negative = lhs.num * rhs.den < rhs.num * lhs.den;
	return frac<T>(
			negative ? rhs.num * lhs.den - lhs.num * rhs.den : lhs.num * rhs.den - rhs.num * lhs.den,
			negative,
			lhs.den * rhs.den);
}

template<typename T>
frac<T> operator*(const frac<T> &lhs, const frac<T> &rhs)
{
	return frac<T>(
			lhs.num * rhs.num,
			!lhs.neg == rhs.neg,
			lhs.den * rhs.den);
}

template<typename T>
frac<T> operator/(const frac<T> &lhs, const frac<T> &rhs)
{
	return lhs * rhs.inverse();
}

template<typename T>
inline frac<T> & frac<T>::operator+=(const frac<T> &rhs)
{
	return *this = *this + rhs;
}

template<typename T>
inline frac<T> & frac<T>::operator-=(const frac<T> &rhs)
{
	return *this = *this - rhs;
}

template<typename T>
inline frac<T> & frac<T>::operator*=(const frac<T> &rhs)
{
	return *this = *this * rhs;
}

template<typename T>
inline frac<T> & frac<T>::operator/=(const frac<T> &rhs)
{
	return *this = *this / rhs;
}

template<typename OutputStream, typename T>
inline OutputStream & operator<<(OutputStream &os, const frac<T> &f)
{
	if (f.neg && f.num != 0)
		os << '-';
	os << f.num;
	if (f.den != 1)
		os << '/' << f.den;
	return os;
}

template<typename InputStream, typename T>
inline InputStream & operator>>(InputStream &is, frac<T> &f)
{
	std::wstring s;
	T num, den = 1;
	bool neg = false;
	is >> s;
	if (s[0] == L'-')
	{
		neg = true;
		s = s.substr(1);
	}
	size_t slash_pos = s.find_first_of(L'/');
	std::wistringstream iss;
	iss.str(s.substr(0, slash_pos));
	iss >> num;
	if (slash_pos != std::wstring::npos)
	{
		iss.seekg(std::ios::beg);
		iss.str(s.substr(slash_pos + 1));
		iss >> den;
	}
	f = frac<T>(num, neg, den);
	return is;
}

#endif
