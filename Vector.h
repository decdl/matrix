
#ifndef _VECTOR_ROW_
#define _VECTOR_ROW_

#include <vector>
#include <cstddef>
#include <utility>

template<typename T>
class Vector : private std::vector<T*>
{
	public:

		// iterator and const_iterator definition
		template<bool citer>
		class _iter
		{
			private:
				typedef typename std::conditional<citer, typename std::vector<T*>::const_iterator, typename std::vector<T*>::iterator>::type base_iterator;
			public:
				
				// basic member types
				typedef T value_type;
				typedef typename base_iterator::difference_type difference_type;
				typedef T* pointer;
				typedef T &reference;
				typedef std::random_access_iterator_tag iterator_category;

				// default constructor
				inline _iter() : iter() {}
				
				// copy constructor with rvalue
				inline _iter(_iter &&rhs) : iter(rhs.iter) {}

				// copy constructor with lvalue
				inline _iter(const _iter &rhs) : iter(rhs.iter) {}

				// constructor with rvalue base_iterator
				inline _iter(base_iterator &&rhs) : iter(rhs) {}

				// constructor with lvalue base_iterator
				inline _iter(base_iterator &rhs) : iter(rhs) {}

				// copy assign operator with rvalue
				inline _iter & operator=(_iter &&rhs) {iter = rhs.iter; return *this;}

				// copy assign operator with lvalue
				inline _iter & operator=(const _iter &rhs) {iter = rhs.iter; return *this;}

				// equal
				inline bool operator==(const _iter &rhs) const {return iter == rhs.iter;}

				// inequal
				inline bool operator!=(const _iter &rhs) const {return iter != rhs.iter;}

				// const dereferencing
				inline const T operator*() const {return **iter;}

				// dereferencing
				inline T & operator*() {return **iter;}

				// member access
				inline T * operator->() const {return *iter;}

				// pre-increament
				inline _iter & operator++() {++iter; return *this;}

				// post-increament
				inline _iter operator++(int) {_iter copy(*this); ++iter; return copy;}

				// pre-decreament
				inline _iter & operator--() {--iter; return *this;}

				// post-decreament
				inline _iter operator--(int) {_iter copy(*this); --iter; return copy;}

				// compound adding an offset
				inline _iter & operator+=(const difference_type &offset) {iter += offset; return *this;}

				// compound subtracting an offset
				inline _iter & operator-=(const difference_type &offset) {iter -= offset; return *this;}

				// adding an offset
				inline _iter operator+(const difference_type &offset) const {return _iter(*this) += offset;}

				// added by an offset
				friend inline _iter operator+(const difference_type &offset, _iter copy) {return copy += offset;}

				// subtracting an offset
				inline _iter operator-(const difference_type &offset) const {return _iter(*this) -= offset;}

				// subtracting an iterator
				inline difference_type operator-(const _iter &rhs) const {return iter - rhs.iter;}

				// less than
				inline bool operator<(const _iter &rhs) const {return iter < rhs.iter;}

				// greater than
				inline bool operator>(const _iter &rhs) const {return iter > rhs.iter;}

				// less than or equal to
				inline bool operator<=(const _iter &rhs) const {return iter <= rhs.iter;}

				// greater than or equal to
				inline bool operator>=(const _iter &rhs) const {return iter >= rhs.iter;}

				// const offset dereferencing
				inline const T operator[](const difference_type &offset) const {return *iter[offset];}

				// offset dereferencing
				inline T operator[](const difference_type &offset) {return *iter[offset];}

			private:
				base_iterator iter;
				friend Vector;
		};
		typedef _iter<true> const_iterator;
		typedef _iter<false> iterator;


		// constructors

		// default constructor
		inline Vector() {}

		// copy-constructor with rvlaue
		inline Vector(Vector &&v) : std::vector<T*>(v) {}

		// copy-constructor with lvalue
		inline Vector(const Vector &v) : std::vector<T*>(v) {}

		// construct from a std::vector
		inline Vector(std::vector<T> &v)
		{
			for (T &e : v)
				push_back(e);
		}


		// modifiers

		// adding const components
		inline Vector & push_back(const T &c)
		{
			std::vector<T*>::push_back(&c);
			return *this;
		}

		// adding components
		inline Vector & push_back(T &c)
		{
			std::vector<T*>::push_back(&c);
			return *this;
		}

		// copy-assign operator with rvalue
		inline Vector & operator=(Vector &&v)
		{
			std::vector<T*>::operator=(v);
			return *this;
		}

		// copy-assign operator with lvalue
		inline Vector & operator=(const Vector &v)
		{
			std::vector<T*>::operator=(v);
			return *this;
		}

		// return a copy of underlying data
		inline std::vector<T> copy() const
		{
			std::vector<T> v;
			for (T &x : *this)
				v.emplace_back(x);
			return v;
		}

		// cast to std::vector
		inline operator std::vector<T>() const {return copy();}

		// begin
		inline iterator begin()
		{
			return iterator(std::vector<T*>::begin());
		}

		// end
		inline iterator end()
		{
			return iterator(std::vector<T*>::end());
		}

		// const begin
		inline const_iterator begin() const
		{
			return const_iterator(std::vector<T*>::begin());
		}

		// const end
		inline const_iterator end() const
		{
			return const_iterator(std::vector<T*>::end());
		}

		// size
		size_t size() const
		{
			return std::vector<T*>::size();
		}

		// const offset dereferencing
		inline const T & operator[](const typename std::vector<T*>::difference_type &n) const
		{
			return *std::vector<T*>::operator[](n);
		}

		// offset dereferencing
		inline T & operator[](const typename std::vector<T*>::difference_type &n)
		{
			return *std::vector<T*>::operator[](n);
		}

		// compound divided by a scalar
		template<typename scalar>
		inline Vector & operator/=(scalar c)
		{
			for (T &e : *this)
				e /= c;
			return *this;
		}

		// divided by a scalar
		template<typename scalar>
		inline std::vector<T> operator/(scalar c) const
		{
			std::vector<T> result(begin(), end());
			Vector(result) /= c;
			return result;
		}

		// compound multipling a scalar
		template<typename scalar>
		inline Vector & operator*=(scalar c)
		{
			for (T &e : *this)
				e *= c;
			return *this;
		}

		// multipling a scalar
		template<typename scalar>
		inline std::vector<T> operator*(scalar c) const
		{
			std::vector<T> result(begin(), end());
			Vector(result) *= c;
			return result;
		}

		// multiplied by a scalar
		template<typename scalar>
		friend inline std::vector<T> operator*(scalar c, const Vector &v)
		{
			std::vector<T> result(v.begin(), v.end());
			Vector(result) *= c;
			return result;
		}

		// compound adding a Vector
		inline Vector & operator+=(const Vector &rhs)
		{
			if (size() != rhs.size())
				throw L"Exception: Vector adding with different dimensions";
			iterator it0 = begin();
			const_iterator it1 = rhs.begin();
			while (it0 != end())
				*(it0++) += *(it1++);
			return *this;
		}

		// adding Vectors
		inline std::vector<T> operator+(const Vector &rhs) const
		{
			std::vector<T> result(begin(), end());
			Vector(result) += rhs;
			return result;
		}

		// compound subtracting a Vector
		inline Vector & operator-=(const Vector &rhs)
		{
			if (size() != rhs.size())
				throw L"Exception: Vector adding with different dimensions";
			iterator it0 = begin();
			const_iterator it1 = rhs.begin();
			while (it0 != end())
				*(it0++) -= *(it1++);
			return *this;
		}

		// subtracting Vectors
		inline std::vector<T> operator-(const Vector &rhs) const
		{
			std::vector<T> result(begin(), end());
			Vector(result) -= rhs;
			return result;
		}

		// dot product
		inline T dot(const Vector &rhs) const
		{
			if (size() != rhs.size())
				throw L"Exception: dot product between different dimensions";
			T result = static_cast<T>(0);
			const_iterator it0 = begin(),
						   it1 = rhs.begin();
			while (it0 != end())
				result += *(it0++) * *(it1++);
			return result;
		}
};

#endif
