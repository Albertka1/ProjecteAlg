#ifndef diccionari_hpp
#define diccionari_hpp

#include <fstream>
#include <random>
#include <string>
#include <vector>

namespace diccionari {
	// Tipus dels nombres a utilitzar (unsigned de 64bit)
	typedef unsigned long long int paraula;

	int read(std::string filename, std::vector<paraula> listNum) {
		std::ifstream fs(filename);
		std::string num;

		if (!fs.is_open())
			return -1;

		while (getline(fs, num))
			listNum.push_back(stoll(num));

		fs.close();
	}

	int generate(std::string filename, int size, int seed) {
		std::ofstream fs(filename);
		paraula tamanoMax = 10000000000000000000;
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
		std::vector<paraula> pars;

	public:
		Diccionari(std::string filename) {
			int i = read(filename, pars);
			if (i == -1) throw;
		}

		virtual bool exists(paraula p) = 0;
	};
}

#endif
