#ifndef arxius_hpp
#define arxius_hpp

#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "paraula.hpp"

namespace diccionari {
    std::vector<std::string> paraulesDicc;
        
	struct {
	private:
		std::uniform_int_distribution<paraula> paraules = std::uniform_int_distribution<paraula>(0, PARAULA_MAX - 1);
		std::uniform_int_distribution<int> percentatge = std::uniform_int_distribution<int>(0, 99);
		std::default_random_engine gen = std::default_random_engine();

	public:
		void llavor(time_t p) { gen.seed((unsigned)p); }
		paraula paraula_aleatoria() { return paraules(gen); }
        template <typename T>
		T seleccio_aleatoria(const std::vector<T>& v) {
            std::uniform_int_distribution<int> dist = std::uniform_int_distribution<int>(0, v.size() - 1);
            int index = dist(gen);
			return v[index];
        }
		template <typename T>
		T genera(T min, T max) {
			std::uniform_int_distribution<T> dist = std::uniform_int_distribution<T>(min, max);
			return dist(gen);
		}

	} motorAleatori;

	std::vector<paraula> genera_diccionari(int size) {
		std::vector<paraula> pars = std::vector<paraula>();

		// Generem les paraules del diccionari aleatoriament
		for (int i = 0; i < size; ++i)
			pars.push_back(motorAleatori.paraula_aleatoria());

		return pars;
	}

	std::vector<paraula> genera_text(int size, float proportion, const std::vector<paraula>& dicc) {
		const int minDiccWords = (int)(proportion * (float)size);
		std::vector<int> diccPos = std::vector<int>();
		std::vector<paraula> text = std::vector<paraula>(size);

		for (int i = 0; i < minDiccWords; ++i)
			diccPos.push_back(motorAleatori.genera<int>(0, size - 1 - minDiccWords));
		std::sort(diccPos.begin(), diccPos.end());
		diccPos.push_back(-1);

		for (int i = 0, j = 0, k = 0; i < size; ++i) {
			if (j == diccPos[k]) {
				text[i] = motorAleatori.seleccio_aleatoria(dicc);
				++k;
			}
			else {
				text[i] = motorAleatori.paraula_aleatoria();
				++j;
			}
		}

		return text;
	}
}

#endif
