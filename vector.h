
#ifndef _VECTOR_ROW_
#define _VECTOR_ROW_

#include <vector>
#include <cstddef>
#include <utility>

template<typename T>
class vector : private std::vector<T>
{
	public:
		typedef typename std::vector<T>::const_iterator const_iterator;
		typedef typename std::vector<T>::iterator iterator;

		// default constructor
		vector() : std::vector<T>() {}

		// copy-constructor with rvlaue
		vector(vector &&v) : std::vector<T>(v) {}

		// copy-constructor with lvalue
		vector(const vector &v) : std::vector<T>(v) {}

		// constructor with rvalue
		template<typename... Args>
		explicit vector(Args&&... args) : std::vector<T>(args...) {}

		// constructor with lvalue
		template<typename... Args>
		explicit vector(const Args&... args) : std::vector<T>(args...) {}

		// copy-assign operator with rvalue
		inline vector &operator=(vector &&v)
		{
			std::vector<T>::operator=(v);
			return *this;
		}

		// copy-assign operator with lvalue
		inline vector &operator=(const vector &v)
		{
			std::vector<T>::operator=(v);
			return *this;
		}

		// begin
		inline iterator begin()
		{
			return std::vector<T>::begin();
		}

		// end
		inline iterator end()
		{
			return std::vector<T>::end();
		}

		// const begin
		inline const_iterator begin() const
		{
			return std::vector<T>::begin();
		}

		// const end
		inline const_iterator end() const
		{
			return std::vector<T>::end();
		}

		// size
		size_t size() const
		{
			return std::vector<T>::size();
		}

		// emplace_back
		template<typename... Args>
		inline void emplace_back(Args&&... args)
		{
			std::vector<T>::emplace_back(args...);
		}

		// const reference from subscript access
		template<typename size_type>
		inline const T &operator[](size_type n) const
		{
			return std::vector<T>::operator[](n);
		}

		// reference from subscript access
		template<typename size_type>
		inline T &operator[](size_type n)
		{
			return std::vector<T>::operator[](n);
		}

		// scalar division
		template<typename scalar>
		inline vector &operator/=(scalar c)
		{
			for (T &e : *this)
				e /= c;
			return *this;
		}
};

// scalar multiplication
template<typename T, typename scalar>
inline vector<T> operator*(const scalar &c, const vector<T> &v)
{
	vector<T> row(v);
	for (T &e : row)
		e *= c;
	return std::move(row);
}

// vector addition
template<typename T>
inline vector<T> operator+(const vector<T> &lhs, const vector<T> &rhs)
{
	if (lhs.size() != rhs.size())
		throw L"Exception: vector adding with different dimensions";
	vector<T> row(lhs);
	typename vector<T>::iterator it0 = row.begin(),
									 it1 = rhs.begin();
	while (it0 != row.end())
		*(it0++) += *(it1++);
	return std::move(row);
}

// vector subtraction
template<typename T>
inline vector<T> operator-(const vector<T> &lhs, const vector<T> &rhs)
{
	if (lhs.size() != rhs.size())
		throw L"Exception: vector subtracting with different dimensions";
	vector<T> row(lhs);
	typename vector<T>::iterator it0 = row.begin();
	typename vector<T>::const_iterator it1 = rhs.begin();
	while (it0 != row.end())
		*(it0++) -= *(it1++);
	return std::move(row);
}

#endif
