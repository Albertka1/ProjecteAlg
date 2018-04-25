#ifndef diccionari_hpp
#define diccionari_hpp

#include <algorithm>
#include <iterator>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

#include "paraula.hpp"

#include "file_io.hpp"

#define macro_stringCast operator std::string() const { return toString(pars); }

namespace diccionari {
	template<class cont, class elem>
	bool conte(const cont& c, const elem& e) { return find(c.cbegin(), c.cend(), e) != c.cend(); }

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
		std::set<paraula> pars;
	public:
		SetFind(const std::vector<paraula>& v) : Diccionari() { pars = std::set<paraula>(v.begin(), v.end()); }

		macro_stringCast
			bool optimitza_lot() const { return true; } // Passar a true quan i_existeix_lot implementada

		bool existeix(paraula p) const { return (pars.find(p) != pars.end()); }
		std::vector<bool> existeix_lot(const std::vector<paraula>& lot) const {
			std::vector<paraula> falten(0);
			std::vector<bool> res(lot.size());

			std::set_difference(lot.cbegin(), lot.cend(), pars.cbegin(), pars.cend(), std::inserter(falten, falten.begin()));
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
	};

	class USetFind : public Diccionari {
	private:
		std::unordered_set<paraula> pars;
	public:
		USetFind(const std::vector<paraula>& v) : Diccionari() { pars = std::unordered_set<paraula>(v.begin(), v.end()); }

		macro_stringCast
			bool existeix(paraula p) const { return (pars.find(p) != pars.end()); }
	};

	Diccionari* factory(int type, const std::string& filename) {
		Diccionari* d;
		std::vector<paraula> pars = std::vector<paraula>();
		utils::read_f(filename, pars);
		switch (type) {
		case  1: d = new CercaSequencial(pars); break;
		case  2: d = new SetFind(pars); break;
		case  3: d = new USetFind(pars); break;
		default: d = NULL;
		}

		return d;
	}

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
