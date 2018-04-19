#ifndef diccionari_hpp
#define diccionari_hpp

#include <fstream>
#include <iterator>
#include <set>
#include <string>
#include <vector>
#include <unordered_set>

#include "paraula.hpp"

namespace diccionari {
	static int read(const std::string& filename, std::vector<paraula>& listNum) {
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
		void init(cont& c, std::string filename) {
			std::vector<paraula> pars(0);
			int i = read(filename, pars);
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
		virtual bool exists(paraula p) = 0;
		virtual operator std::string() = 0;
	};

	class SequentialSearch : public Diccionari {
	private:
		std::vector<paraula> pars;
	public:
		SequentialSearch(std::string filename) : Diccionari() {
			pars = std::vector<paraula>(0);
			init(pars, filename);
		}
		bool exists(paraula p) {
			for (paraula f : pars) if (f == p) return true;
			return false;
		}
		operator std::string() { return toString(pars); }
	};

	class SetFind : public Diccionari {
	private:
		std::set<paraula> pars;
	public:
		SetFind(std::string filename) : Diccionari() { init(pars, filename); }
		bool exists(paraula p) { return (pars.find(p) != pars.end()); }
		operator std::string() { return toString(pars); }
	};

	class USetFind : public Diccionari {
	private:
		std::unordered_set<paraula> pars;
	public:
		USetFind(std::string filename) : Diccionari() { init(pars, filename); }
		bool exists(paraula p) { return (pars.find(p) != pars.end()); }
		operator std::string() { return toString(pars); }
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
