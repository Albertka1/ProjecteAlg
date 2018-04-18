#ifndef diccionari_hpp
#define diccionari_hpp

#include <fstream>
#include <random>
#include <string>
#include <vector>

namespace diccionari {
	// Tipus dels nombres a utilitzar (unsigned de 64bit)
	typedef unsigned long long paraula;

	int read(const std::string& filename, std::vector<paraula>& listNum) {
		std::ifstream fs(filename);
		std::string num;

		if (!fs.is_open()) return -1;

		while (getline(fs, num))
			listNum.push_back(std::stoull(num));

		fs.close();

		return 0;
	}

	int generate(const std::string& filename, int size, int seed) {
		std::ofstream fs(filename);
		paraula tamanoMax = 10000000000000000000;
		std::default_random_engine generator(seed);
		std::uniform_int_distribution<paraula> distribution(0, tamanoMax - 1);

		if (!fs.is_open()) return -1;

		// Generamos los números aleatorios
		for (int i = 0; i < size; ++i) {
			paraula rand = distribution(generator);
			fs << std::to_string(rand);
			fs << std::endl;
		}

		fs.close();

		return 0;
	}

	class Diccionari {
	protected:
		std::vector<paraula> pars;

	public:
		Diccionari(const std::string& filename) {
			pars = std::vector<paraula>(0);
			int i = read(filename, pars);
			if (i == -1) throw;
		}

		virtual bool exists(paraula p) = 0;
	};

	class SequentialSearch : public Diccionari {
	public:
		SequentialSearch(const std::string& filename) : Diccionari(filename) {}

		bool exists(paraula a) {
			for (paraula b : pars)
				if (a == b) return true;
			return false;
		}
	};

	Diccionari* factory(int type, const std::string& filename) {
		Diccionari* d;
		switch (type) {
		case 1:
			d = new SequentialSearch(filename);
			break;
		default:
			d = NULL;
		}

		return d;
	}
}

#endif
