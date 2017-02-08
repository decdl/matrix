
#ifndef _VECTOR_ROW_
#define _VECTOR_ROW_

#include <vector>
#include <cstddef>
#include <utility>

template<typename T>
class vector_row : private std::vector<T>
{
	public:
		typedef typename std::vector<T>::const_iterator const_iterator;
		typedef typename std::vector<T>::iterator iterator;

		// default constructor
		vector_row() : std::vector<T>() {}

		// copy-constructor with rvlaue
		vector_row(vector_row &&v) : std::vector<T>(v) {}

		// copy-constructor with lvalue
		vector_row(const vector_row &v) : std::vector<T>(v) {}

		// constructor with rvalue
		template<typename... Args>
		explicit vector_row(Args&&... args) : std::vector<T>(args...) {}

		// constructor with lvalue
		template<typename... Args>
		explicit vector_row(const Args&... args) : std::vector<T>(args...) {}

		// copy-assign operator with rvalue
		inline vector_row &operator=(vector_row &&v)
		{
			std::vector<T>::operator=(v);
			return *this;
		}

		// copy-assign operator with lvalue
		inline vector_row &operator=(const vector_row &v)
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
		inline vector_row &operator/=(scalar c)
		{
			for (T &e : *this)
				e /= c;
			return *this;
		}
};

// scalar multiplication
template<typename T, typename scalar>
inline vector_row<T> operator*(const scalar &c, const vector_row<T> &v)
{
	vector_row<T> row(v);
	for (T &e : row)
		e *= c;
	return std::move(row);
}

// vector addition
template<typename T>
inline vector_row<T> operator+(const vector_row<T> &lhs, const vector_row<T> &rhs)
{
	if (lhs.size() != rhs.size())
		throw L"Exception: vector adding with different dimensions";
	vector_row<T> row(lhs);
	typename vector_row<T>::iterator it0 = row.begin(),
									 it1 = rhs.begin();
	while (it0 != row.end())
		*(it0++) += *(it1++);
	return std::move(row);
}

// vector subtraction
template<typename T>
inline vector_row<T> operator-(const vector_row<T> &lhs, const vector_row<T> &rhs)
{
	if (lhs.size() != rhs.size())
		throw L"Exception: vector subtracting with different dimensions";
	vector_row<T> row(lhs);
	typename vector_row<T>::iterator it0 = row.begin();
	typename vector_row<T>::const_iterator it1 = rhs.begin();
	while (it0 != row.end())
		*(it0++) -= *(it1++);
	return std::move(row);
}

#endif
