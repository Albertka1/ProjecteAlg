#ifndef diccionari_contenidors_stl_hpp
#define diccionari_contenidors_stl_hpp

#include <functional>
#include <iterator>
#include <set>
#include <unordered_set>
#include <vector>

#include "diccionari.hpp"

namespace diccionari {
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
		
		bool existeix(paraula p) const {
			bool res = pars.find(p) != pars.end();
			return res;
		}
		
		bool optimitza_lot_ordenat() const { return true; }
		std::vector<bool> existeix_lot_ordenat(const std::vector<paraula>& lot) const {
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
}

#endif
