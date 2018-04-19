#ifndef diccionari_hpp
#define diccionari_hpp

#include <algorithm>
#include <fstream>
#include <iterator>
#include <random>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

namespace diccionari {
	// Tipus dels nombres a utilitzar (unsigned de 64bit)
	typedef unsigned long long int paraula;

	int read(std::string filename, std::vector<paraula>& listNum) {
		std::ifstream fs(filename);
		std::string num;

		if (!fs.is_open())
			return -1;

		while (getline(fs, num))
			listNum.push_back(std::stoull(num));

		fs.close();
	}

	int generate(std::string filename, int size, int seed) {
		std::ofstream fs(filename);
		paraula tamanoMax = 1000000000000000000;
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<paraula> distribution(0, tamanoMax - 1);

		if (!fs.is_open())
			return -1;

		// Generamos los números aleatorios
		for (int i = 0; i < size; ++i) {
			paraula rand = distribution(generator);
			fs << std::to_string(rand);
			fs << std::endl;
		}

		fs.close();
	}
	
	class Diccionari {
	protected:
		Diccionari(){}
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
	
	class SeqDicc : public Diccionari {
	private:
		std::vector<paraula> pars;
	public:
		SeqDicc(std::string filename) : Diccionari() {
			pars = std::vector<paraula>(0);
			init(pars, filename);
		}
		bool exists(paraula p) {
			for (paraula f : pars) if (f == p) return true;
			return false;
		}
		operator std::string() { return toString(pars); }
	};
	
	class SetDicc : public Diccionari {
	private:
		std::set<paraula> pars;
	public:
		SetDicc(std::string filename) : Diccionari() { init(pars, filename); }
		bool exists(paraula p) { return (pars.find(p) != pars.end()); }
		operator std::string() { return toString(pars); }
	};
	
	class USetDicc : public Diccionari {
	private:
		std::unordered_set<paraula> pars;
	public:
		USetDicc(std::string filename) : Diccionari() { init(pars, filename); }
		bool exists(paraula p) { return (pars.find(p) != pars.end()); }
		operator std::string() { return toString(pars); }
	};
}

#endif
