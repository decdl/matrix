
#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <string>
#include <istream>
#include <ostream>
#include <sstream>
#include <vector>
#include "Vector.h"

template<typename T>
class Matrix
{
	public:
		// constructors
		Matrix() {}
		Matrix(size_t num_row, size_t num_col) : e(num_row, std::vector<T>(num_col)) {}
		Matrix(const Matrix &A) : e(A.e) {}
		Matrix(Matrix &&A) : e(A.e) {}

		// assign operators
		Matrix & operator=(const Matrix &A) {e = A.e; return *this;}

		// clear
		Matrix & clear() {e.clear(); return *this;}

		// resize
		Matrix & resize(size_t r, size_t c)
		{
			e.resize(r);
			for (std::vector<T> &v : e)
				v.resize(c);
			return *this;
		}

		// reserve
		Matrix & reserve_row(size_t r)
		{
			e.reserve(r);
			return *this;
		}
		Matrix & reserve_col(size_t c)
		{
			for (std::vector<T> &v : e)
				v.reserve(c);
			return *this;
		}

		// add rows
		Matrix & add_row(const Vector<T> &v)
		{
			if (v.size() != col())
				throw L"Exception: adding rows with different dimensions";
			e.emplace_back(v);
			return *this;
		}

		// add cols
		Matrix & add_col(const Vector<T> &v)
		{
			if (v.size() != row())
				throw L"Exception: adding cols with different dimensions";
			for (size_t i = 0; i < v.size(); i++)
				e[i].emplace_back(v[i]);
			return *this;
		}

		// accessor
		T & get(size_t r, size_t c) {return e[r][c];}
		const T & get(size_t r, size_t c) const {return e[r][c];}

		// access const rows
		Vector<T const> row(size_t r) const
		{
			Vector<T const> v;
			for (const T &c : e[r])
				v.push_back(c);
			return v;
		}

		// access rows
		Vector<T> row(size_t r)
		{
			Vector<T> v;
			for (T &c : e[r])
				v.push_back(c);
			return v;
		}

		// access number of rows
		size_t row() const {return e.size();}

		// access const cols
		Vector<T const> col(size_t c) const
		{
			Vector<T const> v;
			for (const std::vector<T> &row : e)
				v.push_back(e[c]);
			return v;
		}

		// access cols
		Vector<T> col(size_t c)
		{
			Vector<T> v;
			for (std::vector<T> &row : e)
				v.push_back(e[c]);
			return v;
		}

		// access number of cols
		size_t col() const {if (e.empty()) return 0; else return e.front().size();}

		// input
		template<typename Char>
		friend std::basic_istream<Char> & operator>>(std::basic_istream<Char> &is, Matrix &A)
		{
			A.e.clear();
			std::basic_string<Char> row;
			std::basic_istringstream<Char> iss;
			size_t cols = 0;
			while (std::getline(is, row) && !row.empty())
			{
				iss.str(row);
				iss.seekg(0);
				A.e.emplace_back();
				size_t count;
				for (count = 0; !iss.eof(); count++)
				{
					T entry;
					iss >> entry;
					A.e.back().push_back(std::move(entry));
				}
				if (cols == 0)
					cols = count;
				else if (cols != count)
				{
					is.clear(std::ios::failbit);
					return is;
				}
			}
			return is;
		}

		// output
		template<typename Char>
		friend std::basic_ostream<Char> & operator<<(std::basic_ostream<Char> &os, const Matrix &A)
		{
			for (const std::vector<T> &v : A.e)
			{
				bool first_entry = true;
				for (const T &entry : v)
				{
					if (first_entry)
						first_entry = false;
					else
						os << static_cast<Char>(L'\t');
					os << entry;
				}
				os << std::endl;
			}
			return os;
		}

		// line reduce into REF
		Matrix & reduce_to_ref(size_t aug = 0)
		{
			size_t leading = 0;
			for (size_t j = 0; leading < row() && j < col() - aug; j++)
			{
				if (e[leading][j] == static_cast<T>(0))
				{
					for (size_t i = leading + 1; i < row(); i++)
						if (e[i][j] != static_cast<T>(0))
						{
							std::vector<T> tmp = std::move(e[leading]);
							e[leading] = std::move(e[i]);
							e[i] = std::move(tmp);
							break;
						}
					if (e[leading][j] == static_cast<T>(0))
						continue;
				}
				row(leading) /= e[leading][j];
				for (size_t i = 0; i < row(); i++)
					if (i != leading)
					{
						auto result = e[i][j] * row(leading);
						row(i) -= Vector<T>(result);
					}
				leading++;
			}
			return *this;
		}

		// get the reduced form of the Matrix
		Matrix ref(size_t aug = 0) const {return Matrix(*this).reduce_to_ref(aug);}

		// determinant
		// use the naive method
		T det() const
		{
			if (row() != col())
				throw L"Exception: determinant of non-square Matrix";
			if (row() == 1)
				return e[0][0];

			T result = static_cast<T>(0);
			for (size_t j = 0; j < col(); j++)
			{
				// calculate cofactor
				Matrix A(row()-1, 0);
				for (size_t i = 0; i < A.row(); i++)
				{
					A.e[i].reserve(col() - 1);
					A.e[i].insert(A.e[i].end(), e[i+1].begin(), e[i+1].begin() + j);
					A.e[i].insert(A.e[i].end(), e[i+1].begin() + j + 1, e[i+1].end());
				}
				result += (j%2==0 ? e[0][j] : -e[0][j]) * A.det();
			}
			return result;
		}

	private:
		std::vector<std::vector<T>> e;
};

#endif

