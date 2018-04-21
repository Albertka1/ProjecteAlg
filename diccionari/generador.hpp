#ifndef arxius_hpp
#define arxius_hpp

#include <fstream>
#include <random>
#include <string>
#include <vector>

#include "paraula.hpp"

namespace diccionari {
	paraula tamanyMax = 10000000000000000000;

	struct {
	private:
		std::uniform_int_distribution<paraula> dist = std::uniform_int_distribution<paraula>(0, tamanyMax - 1);
		std::default_random_engine gen = std::default_random_engine();

	public:
		void llavor(time_t p) { gen.seed((unsigned)p); }
		paraula genera() { return dist(gen); }
	} motorAleatori;

	int genera_diccionari(const std::string& filename, int size) {
		std::ofstream fs(filename);

		if (!fs.is_open()) return -1;

		// Generem les paraules del diccionari aleatoriament
		for (int i = 0; i < size; ++i) {
			paraula aleatoria = motorAleatori.genera();
			fs << std::to_string(aleatoria);
			fs << std::endl;
		}

		fs.close();
		return 0;
	}

}

#endif
