#ifndef diccionari_diccionari_hpp
#define diccionari_diccionari_hpp

#include <algorithm>
#include <string>

#include "paraula.hpp"

namespace diccionari {
	template<class cont, class elem>
	bool conte(const cont& c, const elem& e) { return std::find(c.cbegin(), c.cend(), e) != c.cend(); }
	
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
	
	class Diccionari {
	protected:
		Diccionari() {}
		
		template<class cont>
		static std::string toString(const cont& c) {
			std::string s = "";
			for (paraula p : c) s += std::to_string(p) + '\n';
			return s;
		}
		#define macro_stringCast operator std::string() const { return Diccionari::toString(pars); } // pars es un contenidor iterable
		
	public:
		virtual operator std::string() const = 0;
		
		virtual bool existeix(paraula p) const = 0;
		
		virtual bool optimitza_lot() const { return false; }
		virtual std::vector<bool> existeix_lot(std::vector<paraula>& lot) const { // POST: lot està ordenat
			std::vector<bool> res(0);
			std::sort(lot.begin(), lot.end());
			auto it = std::unique(lot.begin(), lot.end());
			lot.resize(it - lot.begin());
			for (paraula p : lot) res.push_back(existeix(p));
			return res;
		}
		
		virtual bool optimitza_lot_ordenat() const { return false; }
		virtual std::vector<bool> existeix_lot_ordenat(const std::vector<paraula>& lot) const { // PRE: lot està ordenat
			std::vector<bool> res(0);
			for (paraula p : lot) res.push_back(existeix(p));
			return res;
		}
		
		virtual long long int count_comps() const { return -1; }
		virtual void restart_count() {}

		virtual float getLoadFactor() { return -1; }
	};
	
}

#endif
