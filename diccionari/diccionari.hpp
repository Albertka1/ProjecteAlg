#ifndef diccionari_hpp
#define diccionari_hpp

#include <fstream>
#include <iterator>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>

#include "paraula.hpp"

#define macro_stringCast operator std::string() { return toString(pars); }

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
		std::string toString(const cont& c) {
			std::string s = "";
			for (paraula p : c) s += std::to_string(p) + '\n';
			return s;
		}
	public:
		virtual operator std::string() = 0;
		virtual bool optimitza_lot() { return false; }

		virtual bool existeix(paraula p) = 0;
		virtual std::vector<bool> existeix_lot(const std::vector<paraula>& lot) {
			std::vector<bool> res(0);
			for (paraula p : lot) res.push_back(existeix(p));
			return res;
		}
	};

	class SequentialSearch : public Diccionari {
	private:
		std::vector<paraula> pars;
	public:
		SequentialSearch(const std::string& filename) : Diccionari() {
			pars = std::vector<paraula>(0);
			init(pars, filename);
		}

		macro_stringCast
		bool existeix(paraula p) {
			for (paraula f : pars) if (f == p) return true;
			return false;
		}
	};

	class SetFind : public Diccionari {
	private:
		std::set<paraula> pars;
	public:
		SetFind(const std::string& filename) : Diccionari() { init(pars, filename); }

		macro_stringCast
		bool existeix(paraula p) { return (pars.find(p) != pars.end()); }
	};

	class USetFind : public Diccionari {
	private:
		std::unordered_set<paraula> pars;
	public:
		USetFind(const std::string& filename) : Diccionari() { init(pars, filename); }

		macro_stringCast
		bool existeix(paraula p) { return (pars.find(p) != pars.end()); }
	};

	Diccionari* factory(int type, const std::string& filename) {
		Diccionari* d;
		switch (type) {
		case 1: d = new SequentialSearch(filename);
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
