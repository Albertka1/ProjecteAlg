#ifndef diccionari_hpp
#define diccionari_hpp

#include <algorithm>
#include <functional>
#include <iterator>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include <iostream>
using namespace std;

#include "paraula.hpp"

#define macro_stringCast operator std::string() const { return toString(pars); }

namespace diccionari {
	template<class cont, class elem>
	bool conte(const cont& c, const elem& e) { return find(c.cbegin(), c.cend(), e) != c.cend(); }

	template <class Comp>
	class comp_count {
	private:
		long long int * comps;
		Comp cmp;
	public:
		comp_count() {}
		comp_count(Comp c, long long int * l) {
			comps = l;
			cmp = c;
		}
		bool operator() (paraula a, paraula b) const {
			(*comps)++;
			return cmp(a, b);
		}
	};
	typedef comp_count<std::less<paraula>> cmp_par;

	template <class Comp>
	comp_count<Comp> make_comparator_counter(Comp c) { return comp_count<Comp>(c); }

	template <class T>
	bool equals(const T& a, const T& b) { return a == b; }
	template <class T>
	bool less(const T& a, const T& b) { return a == b; }

	class Diccionari {
	protected:
		Diccionari() {}
		template<class cont>
		std::string toString(const cont& c) const {
			std::string s = "";
			for (paraula p : c) s += std::to_string(p) + '\n';
			return s;
		}
	public:
		virtual operator std::string() const = 0;
		virtual bool optimitza_lot() const { return false; }

		virtual bool existeix(paraula p) const = 0;

		// PRE: lot està ordenat
		virtual std::vector<bool> existeix_lot(const std::vector<paraula>& lot) const {
			std::vector<bool> res(0);
			for (paraula p : lot) res.push_back(existeix(p));
			return res;
		}

		virtual long long int count_comps() const { return -1; }
		virtual void restart_count() {}
	};

	class CercaSequencial : public Diccionari {
	private:
		std::vector<paraula> pars;
	public:
		CercaSequencial(const std::vector<paraula>& v) : Diccionari() { pars = std::vector<paraula>(v); }

		macro_stringCast
			bool existeix(paraula p) const { return conte(pars, p); }
	};

	class SetFind : public Diccionari {
	private:
		long long int n_comp;
		cmp_par cmp;
		std::set<paraula, cmp_par> pars;

	public:
		SetFind(const std::vector<paraula>& v) : Diccionari() {
			cmp = cmp_par(std::less<paraula>(), &n_comp);
			pars = std::set<paraula, cmp_par>(v.begin(), v.end(), cmp);
			n_comp = 0;
		}

		macro_stringCast
		bool optimitza_lot() const { return true; }

		bool existeix(paraula p) const {
			bool res = pars.find(p) != pars.end();
			return res;
		}
		std::vector<bool> existeix_lot(const std::vector<paraula>& lot) const {
			std::vector<paraula> falten(0);
			std::vector<bool> res(lot.size());

			std::set_difference(lot.cbegin(), lot.cend(), pars.cbegin(), pars.cend(),
				std::inserter(falten, falten.begin()), cmp);
			for (unsigned i = 0, j = 0; i < lot.size() && j < falten.size(); ++i) {
				if (lot[i] == falten[j]) {
					++j;
					res[i] = false;
				}
				else
					res[i] = true;
			}

			return res;
		}

		long long int count_comps() const { return n_comp; }
		void restart_count() { n_comp = 0; }
	};

	class USetFind : public Diccionari {
	private:
		std::unordered_set<paraula> pars;
	public:
		USetFind(const std::vector<paraula>& v) : Diccionari() { pars = std::unordered_set<paraula>(v.begin(), v.end()); }

		macro_stringCast
			bool existeix(paraula p) const { return (pars.find(p) != pars.end()); }
	};

	Diccionari* factory(int type, const std::vector<paraula>& pars) {
		Diccionari* d;
		switch (type) {
		case  1: d = new CercaSequencial(pars); break;
		case  2: d = new SetFind(pars); break;
		case  3: d = new USetFind(pars); break;
		default: d = NULL;
		}

		return d;
	}

}

#endif
