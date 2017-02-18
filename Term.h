
#ifndef _TERM_H_
#define _TERM_H_

#include <map>
#include <initializer_list>
#include <utility>
#include <istream>
#include <ostream>

template<typename Coefficient, typename VarId, typename Exponent = Coefficient>
class Term
{
	private:
		Coefficient c;
		std::map<VarId, Exponent> v;
		typedef std::pair<const VarId, Exponent> pair;
	public:

		// constructors
		inline Term(Coefficient n = 0, std::initializer_list<pair> l = {}) : c(n), v(l) {}
		inline Term(const Term &t) : c(t.c), v(t.v) {}
		inline Term(Term &&t) : c(t.c), v(t.v) {}


		// i/o

		// input
		inline friend std::wistream & operator>>(std::wistream &is, Term &t) {}

		// output
		inline friend std::wostream & operator<<(std::wostream &os, const Term &t)
		{
			bool output_operator = false;
			if (t.c != static_cast<Coefficient>(1) || t.v.size() == 0)
			{
				os << t.c;
				output_operator = true;
			}
			if (t.c != static_cast<Coefficient>(0))
				for (const pair &p : t.v)
				{
					if (output_operator)
						os << L'*';
					os << p.first << '^' << p.second;
					output_operator = true;
				}
			return os;
		}


		// like
		inline bool like(const Term &t) {return v == t.v;}


		// equality
		inline bool operator==(const Term &t) const {return c == t.c && v == t.v;}
		inline bool operator!=(const Term &t) const {return !operator==(t);}

		// relation
		inline bool operator<(const Term &t) const
		{
			typename std::map<VarId, Exponent>::const_iterator i0, i1;
			for (i0 = v.begin(), i1 = t.v.begin(); i0 != v.end() && i1 != t.v.end(); ++i0, ++i1)
				if (*i0 < *i1)
					return true;
				else if (*i0 > *i1)
					return false;
			if (i0 != v.end())
				return false;
			if (i1 != t.v.end())
				return true;
			return c < t.c;
		}

		// operations

		// addition
		inline Term & operator+=(const Term &t)
		{
			if (!like(t))
				throw L"Exception: adding unlike terms";
			c += t.c;
			return *this;
		}
		inline Term operator+(const Term &t) const {return Term(*this) += t;}

		// subtraction
		inline Term & operator-=(const Term &t)
		{
			if (!like(t))
				throw L"Exception: subtracting unlike terms";
			c -= t.c;
			return *this;
		}
		inline Term operator-(const Term &t) const {return Term(*this) -= t;}

		// multiplication
		inline Term & operator*=(const Term &t)
		{
			c *= t.c;
			for (const pair &p : t.v)
				if ((v[p.first] += p.second) == static_cast<Exponent>(0))
					v.erase(p.first);
			return *this;
		}
		inline Term operator*(const Term &t) const {return Term(*this) *= t;}

		// division
		inline Term & operator/=(const Term &t)
		{
			c /= t.c;
			for (const pair &p : t.v)
				if ((v[p.first] -= p.second) == static_cast<Exponent>(0))
					v.erase(p.first);
			return *this;
		}
		inline Term operator/(const Term &t) const {return Term(*this) /= t;}
};

#endif
