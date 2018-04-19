#ifndef arxius_hpp
#define arxius_hpp

#include <fstream>
#include <random>
#include <string>
#include <vector>

#include "paraula.hpp"

namespace diccionari {
	struct {
	private:
		paraula tamanoMax = 10000000000000000000;
		std::uniform_int_distribution<paraula> dist = std::uniform_int_distribution<paraula>(0, tamanoMax - 1);
		std::default_random_engine gen = std::default_random_engine();

	public:
		void seed(paraula p) { gen.seed(p); }
		paraula next() { return dist(gen); }
	} randomEngine;

	int generate(const std::string& filename, int size) {
		std::ofstream fs(filename);

		if (!fs.is_open()) return -1;

		// Generamos los números aleatorios
		for (int i = 0; i < size; ++i) {
			paraula rand = randomEngine.next();
			fs << std::to_string(rand);
			fs << std::endl;
		}

		fs.close();

		return 0;
	}

}

#endif