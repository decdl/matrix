
#ifndef _VECTOR_ROW_
#define _VECTOR_ROW_

#include <vector>
#include <cstddef>
#include <utility>

template<typename T>
class vector : private std::vector<T*>
{
	public:

		// iterator and const_iterator definition
		template<typename T_CV>
		class _iter
		{
			public:
				
				// basic member types
				typedef T_CV value_type;
				typedef typename std::vector<T_CV*>::iterator::difference_type difference_type;
				typedef T_CV *pointer;
				typedef T_CV &reference;
				typedef std::random_access_iterator_tag iterator_category;

				// default constructor
				_iter() : iter() {}
				
				// copy constructor with rvalue
				_iter(_iter<T_CV> &&rhs) : iter(rhs.iter) {}

				// copy constructor with lvalue
				_iter(const _iter<T_CV> &rhs) : iter(rhs.iter) {}

				// constructor with rvalue std::vector<T_CV*>::iterator
				_iter(typename std::vector<T_CV*>::iterator &&rhs) : iter(rhs) {}

				// constructor with lvalue std::vector<T_CV*>::iterator
				_iter(const typename std::vector<T_CV*>::iterator &rhs) : iter(rhs) {}

				// copy assign operator with rvalue
				inline _iter<T_CV> & operator=(_iter<T_CV> &&rhs) {iter = rhs.iter; return *this;}

				// copy assign operator with lvalue
				inline _iter<T_CV> & operator=(const _iter<T_CV> &rhs) {iter = rhs.iter; return *this;}

				// equal
				inline bool operator==(const _iter<T_CV> &rhs) const {return iter == rhs.iter;}

				// inequal
				inline bool operator!=(const _iter<T_CV> &rhs) const {return iter != rhs.iter;}

				// const dereferencing
				inline const T_CV operator*() const {return **iter;}

				// dereferencing
				inline T_CV & operator*() {return **iter;}

				// member access
				inline T_CV * operator->() const {return *iter;}

				// pre-increament
				inline _iter<T_CV> & operator++() {++iter; return *this;}

				// post-increament
				inline _iter<T_CV> operator++(int) {_iter<T_CV> copy(*this); ++iter; return copy;}

				// pre-decreament
				inline _iter<T_CV> & operator--() {--iter; return *this;}

				// post-decreament
				inline _iter<T_CV> operator--(int) {_iter<T_CV> copy(*this); --iter; return copy;}

				// compound adding an offset
				inline _iter<T_CV> & operator+=(const difference_type &offset) {iter += offset; return *this;}

				// compound subtracting an offset
				inline _iter<T_CV> & operator-=(const difference_type &offset) {iter -= offset; return *this;}

				// adding an offset
				inline _iter<T_CV> operator+(const difference_type &offset) const {return _iter<T_CV>(*this) += offset;}

				// added by an offset
				friend inline _iter<T_CV> operator+(const difference_type &offset, _iter<T_CV> copy) {return copy += offset;}

				// subtracting an offset
				inline _iter<T_CV> operator-(const difference_type &offset) const {return _iter<T_CV>(*this) -= offset;}

				// subtracting an iterator
				inline difference_type operator-(const _iter<T_CV> &rhs) const {return iter - rhs.iter;}

				// less than
				inline bool operator<(const _iter<T_CV> &rhs) const {return iter < rhs.iter;}

				// greater than
				inline bool operator>(const _iter<T_CV> &rhs) const {return iter > rhs.iter;}

				// less than or equal to
				inline bool operator<=(const _iter<T_CV> &rhs) const {return iter <= rhs.iter;}

				// greater than or equal to
				inline bool operator>=(const _iter<T_CV> &rhs) const {return iter >= rhs.iter;}

				// const offset dereferencing
				inline const T_CV operator[](const difference_type &offset) const {return *iter[offset];}

				// offset dereferencing
				inline T_CV operator[](const difference_type &offset) {return *iter[offset];}

			private:
				typename std::vector<T_CV*>::iterator iter;
				friend vector;
		};
		typedef _iter<T const> const_iterator;
		typedef _iter<T> iterator;

		// default constructor
		vector() : std::vector<T*>() {}

		// copy-constructor with rvlaue
		vector(vector &&v) : std::vector<T*>(v) {}

		// copy-constructor with lvalue
		vector(const vector &v) : std::vector<T*>(v) {}

		// constructor with rvalue
		template<typename... Args>
		explicit vector(Args&&... args) : std::vector<T*>(args...) {}

		// constructor with lvalue
		template<typename... Args>
		explicit vector(const Args&... args) : std::vector<T*>(args...) {}

		// copy-assign operator with rvalue
		inline vector & operator=(vector &&v)
		{
			std::vector<T*>::operator=(v);
			return *this;
		}

		// copy-assign operator with lvalue
		inline vector & operator=(const vector &v)
		{
			std::vector<T*>::operator=(v);
			return *this;
		}

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
			return iterator(std::vector<T*>::begin());
		}

		// const end
		inline const_iterator end() const
		{
			return iterator(std::vector<T*>::end());
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
		inline vector & operator/=(scalar c)
		{
			for (T &e : *this)
				e /= c;
			return *this;
		}

		// divided by a scalar
		template<typename scalar>
		inline vector operator/(scalar c)
		{
			return vector(*this) /= c;
		}

		// compound multipling a scalar
		template<typename scalar>
		inline vector & operator*=(const scalar &c)
		{
			for (T &e : *this)
				e *= c;
			return *this;
		}

		// multipling a scalar
		template<typename scalar>
		inline vector operator*(const scalar &c)
		{
			return vector(*this) *= c;
		}

		// multiplied by a scalar
		template<typename scalar>
		friend inline vector operator*(const scalar &c, vector v)
		{
			return v *= c;
		}

		// compound adding a vector
		inline vector & operator+=(const vector &rhs)
		{
			if (size() != rhs.size())
				throw L"Exception: vector adding with different dimensions";
			iterator it0 = begin(),
					 it1 = rhs.begin();
			while (it0 != end())
				*(it0++) += *(it1++);
			return *this;
		}

		// adding vectors
		inline vector operator+(const vector &rhs)
		{
			return vector(*this) += rhs;
		}

		// compound subtracting a vector
		inline vector & operator-=(const vector &rhs)
		{
			if (size() != rhs.size())
				throw L"Exception: vector adding with different dimensions";
			iterator it0 = begin(),
					 it1 = rhs.begin();
			while (it0 != end())
				*(it0++) -= *(it1++);
			return *this;
		}

		// subtracting vectors
		inline vector operator-(const vector &rhs)
		{
			return vector(*this) -= rhs;
		}

};

#endif
