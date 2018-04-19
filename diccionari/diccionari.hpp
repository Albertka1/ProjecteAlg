#ifndef diccionari_hpp
#define diccionari_hpp

#include <algorithm>
#include <fstream>
#include <iterator>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>

#include "paraula.hpp"

#define macro_stringCast operator std::string() const { return toString(pars); }

namespace diccionari {
	int llegeix(const std::string& filename, std::vector<paraula>& listNum) {
		std::ifstream fs(filename);
		std::string num;

		if (!fs.is_open()) return -1;

		while (getline(fs, num))
			listNum.push_back(std::stoull(num));

		fs.close();
		return 0;
	}
	
	template<class cont, class elem>
	bool conte(const cont& c, const elem& e) { return find(c.cbegin(), c.cend(), e) != c.cend(); }

	class Diccionari {
	protected:
		Diccionari() {}
		template<class cont>
		void init(cont& c, const std::string& filename) {
			std::vector<paraula> pars(0);
			int i = llegeix(filename, pars);
			if (i == -1) throw;
			std::copy(pars.begin(), pars.end(), inserter(c, c.begin()));
		}
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
		CercaSequencial(const std::string& filename) : Diccionari() {
			pars = std::vector<paraula>(0);
			init(pars, filename);
		}

		macro_stringCast
		bool existeix(paraula p) const { return conte(pars, p); }
	};

	class SetFind : public Diccionari {
	private:
		std::set<paraula> pars;
	public:
		SetFind(const std::string& filename) : Diccionari() { init(pars, filename); }

		macro_stringCast
		bool optimitza_lot() const { return false; } // Passar a true quan i_existeix_lot implementada

		bool existeix(paraula p) const { return (pars.find(p) != pars.end()); }

		typedef std::set<paraula>::const_iterator sp_cit;
		void i_existeix_lot(const std::vector<paraula>& lot, sp_cit ini_pars, sp_cit fi_pars,
			int ini_lot, int fi_lot, std::vector<bool> res) const {
			if (ini_lot == fi_lot);
		}
		std::vector<bool> existeix_lot(const std::vector<paraula>& lot) const {
			std::vector<bool> res(lot.size());
			i_existeix_lot(lot, pars.cbegin(), pars.cend(), 0, lot.size() - 1, res);
			return res;
		}
	};

	class USetFind : public Diccionari {
	private:
		std::unordered_set<paraula> pars;
	public:
		USetFind(const std::string& filename) : Diccionari() { init(pars, filename); }

		macro_stringCast
		bool existeix(paraula p) const { return (pars.find(p) != pars.end()); }
	};

	Diccionari* factory(int type, const std::string& filename) {
		Diccionari* d;
		switch (type) {
		case 1: d = new CercaSequencial(filename);
			break;
		case 2: d = new SetFind(filename);
			break;
		case 3: d = new USetFind(filename);
			break;
		default:
			d = NULL;
		}

		return d;
	}
}

#endif
