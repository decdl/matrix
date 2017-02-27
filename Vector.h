
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdexcept>
#include <vector>
#include <cstddef>
#include <utility>

template<typename T, bool C = false>
class Vector : private std::vector<typename std::conditional<C, const T *, T *>::type>
{
	private:
		typedef typename std::conditional<C, const T, T>::type T_CV;
		class copy_tag{};
		std::vector<T> data;
		inline void construct_self()
		{
			for (T &e : data)
				push_back(e);
		}
		inline Vector(const Vector &rhs, copy_tag) : data(rhs) {construct_self();}
		friend std::vector<T_CV*> & std::vector<T_CV*>::operator=(const std::vector<T_CV*> &);
	public:
		// iterator and const_iterator definition
		template<bool citer>
		class _iter
		{
			private:
				typedef typename std::conditional<citer, typename std::vector<T_CV*>::const_iterator, typename std::vector<T_CV*>::iterator>::type base_iterator;
				base_iterator iter;
				friend Vector;
			public:
				
				// basic member types
				typedef T_CV value_type;
				typedef typename base_iterator::difference_type difference_type;
				typedef T* pointer;
				typedef T_CV &reference;
				typedef std::random_access_iterator_tag iterator_category;

				// default constructor
				inline _iter() : iter() {}
				
				// copy constructor with rvalue
				inline _iter(_iter &&rhs) : iter(std::forward<base_iterator>(rhs.iter)) {}

				// copy constructor with lvalue
				inline _iter(const _iter &rhs) : iter(rhs.iter) {}

				// constructor with rvalue base_iterator
				inline _iter(base_iterator &&rhs) : iter(std::forward<base_iterator>(rhs)) {}

				// constructor with lvalue base_iterator
				inline _iter(base_iterator &rhs) : iter(rhs) {}

				// copy assign operator with rvalue
				inline _iter & operator=(_iter &&rhs) {iter = std::forward(rhs.iter); return *this;}

				// copy assign operator with lvalue
				inline _iter & operator=(const _iter &rhs) {iter = rhs.iter; return *this;}

				// equal
				inline bool operator==(const _iter &rhs) const {return iter == rhs.iter;}

				// inequal
				inline bool operator!=(const _iter &rhs) const {return iter != rhs.iter;}

				// const dereferencing
				inline const T_CV operator*() const {return **iter;}

				// dereferencing
				inline T_CV & operator*() {return **iter;}

				// member access
				inline T_CV * operator->() const {return *iter;}

				// pre-increament
				inline _iter & operator++() {++iter; return *this;}

				// post-increament
				inline _iter operator++(int) {_iter copy(*this); ++iter; return copy;}

				// pre-decreament
				inline _iter & operator--() {--iter; return *this;}

				// post-decreament
				inline _iter operator--(int) {_iter copy(*this); --iter; return copy;}

				// compound addition with an offset
				inline _iter & operator+=(const difference_type &offset) {iter += offset; return *this;}

				// compound subtraction by an offset
				inline _iter & operator-=(const difference_type &offset) {iter -= offset; return *this;}

				// addition with an offset
				inline _iter operator+(const difference_type &offset) const {return _iter(*this) += offset;}
				friend inline _iter operator+(const difference_type &offset, _iter copy) {return copy += offset;}

				// subtraction by an offset
				inline _iter operator-(const difference_type &offset) const {return _iter(*this) -= offset;}

				// subtraction by an iterator
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
				inline const T_CV operator[](const difference_type &offset) const {return *iter[offset];}

				// offset dereferencing
				inline T_CV operator[](const difference_type &offset) {return *iter[offset];}
		};
		typedef _iter<true> const_iterator;
		typedef _iter<false> iterator;


		// constructors

		// default constructor
		inline Vector() {}

		// copy-constructor with rvlaue
		inline Vector(Vector &&v) : data(std::forward<std::vector<T>>(v.data))
		{
			if (own_data())
			{
				v.clear();
				for (T_CV &e : data)
					push_back(e);
			}
			else
				std::vector<T_CV*>::operator=(std::forward<Vector>(v));
		}

		// copy-constructor with lvalue
		inline Vector(const Vector &v) : data(v.data)
		{
			if (own_data())
				for (T_CV &e : data)
					push_back(e);
			else
				std::vector<T_CV*>::operator=(v);
		}

		// construct from a std::vector
		inline Vector(std::vector<T_CV> &v)
		{
			for (T_CV &e : v)
				push_back(e);
		}

		// construct with elements
		inline Vector(size_t n) : data(n)
		{
			for (T_CV &e : data)
				push_back(e);
		}

		// modifiers

		// adding components
		inline Vector & push_back(T_CV &c)
		{
			std::vector<T_CV*>::push_back(&c);
			return *this;
		}

		// copy-assign operator with rvalue
		inline Vector & operator=(Vector &&v)
		{
			std::vector<T_CV*>::operator=(std::forward(v));
			return *this;
		}

		// copy-assign operator with lvalue
		inline Vector & operator=(const Vector &v)
		{
			std::vector<T_CV*>::operator=(v);
			return *this;
		}

		// return a copy of underlying data
		inline Vector copy() const
		{
			return Vector(*this, copy_tag());
		}

		// check if owns data
		inline bool own_data() {return !data.empty();}

		// cast into std::vector
		inline operator std::vector<T>() const
		{
			return std::vector<T>(begin(), end());
		}

		// begin
		inline iterator begin()
		{
			return iterator(std::vector<T_CV*>::begin());
		}

		// end
		inline iterator end()
		{
			return iterator(std::vector<T_CV*>::end());
		}

		// const begin
		inline const_iterator begin() const
		{
			return const_iterator(std::vector<T_CV*>::begin());
		}

		// const end
		inline const_iterator end() const
		{
			return const_iterator(std::vector<T_CV*>::end());
		}

		// size
		size_t size() const
		{
			return std::vector<T_CV*>::size();
		}

		// const offset dereferencing
		inline const T & operator[](const typename std::vector<T_CV*>::difference_type &n) const
		{
			return *std::vector<T_CV*>::operator[](n);
		}

		// offset dereferencing
		inline T & operator[](const typename std::vector<T_CV*>::difference_type &n)
		{
			return *std::vector<T_CV*>::operator[](n);
		}

		// compound division by a scalar
		template<typename scalar>
		inline Vector & operator/=(scalar c)
		{
			for (T &e : *this)
				e /= c;
			return *this;
		}

		// division by a scalar
		template<typename scalar>
		inline Vector operator/(scalar c) const
		{
			return this->copy() /= c;
		}

		// compound multiplication with a scalar
		template<typename scalar>
		inline Vector & operator*=(scalar c)
		{
			for (T &e : *this)
				e *= c;
			return *this;
		}

		// multiplication with a scalar
		template<typename scalar>
		inline Vector operator*(scalar c) const
		{
			return this->copy() *= c;
		}
		template<typename scalar>
		friend inline Vector operator*(scalar c, const Vector &v)
		{
			return v * c;
		}

		// compound addition with a Vector
		inline Vector & operator+=(const Vector &rhs)
		{
			if (size() != rhs.size())
				throw std::invalid_argument("Vector addition with different dimensions");
			iterator it0 = begin();
			const_iterator it1 = rhs.begin();
			while (it0 != end())
				*(it0++) += *(it1++);
			return *this;
		}

		// addition Vectors
		inline Vector operator+(const Vector &rhs) const
		{
			return this->copy() += rhs;
		}

		// compound subtraction by a Vector
		inline Vector & operator-=(const Vector &rhs)
		{
			if (size() != rhs.size())
				throw std::invalid_argument("Vector subtraction with different dimensions");
			iterator it0 = begin();
			const_iterator it1 = rhs.begin();
			while (it0 != end())
				*(it0++) -= *(it1++);
			return *this;
		}

		// subtraction by a Vector
		inline Vector operator-(const Vector &rhs) const
		{
			return this->copy() -= rhs;
		}

		// dot product
		inline T dot(const Vector &rhs) const
		{
			if (size() != rhs.size())
				throw std::invalid_argument("dot product between different dimensions");
			T result = static_cast<T>(0);
			const_iterator it0 = begin(),
						   it1 = rhs.begin();
			while (it0 != end())
				result += *(it0++) * *(it1++);
			return result;
		}
};

#endif
