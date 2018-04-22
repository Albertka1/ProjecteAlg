#include <algorithm>
#include <ctime>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "diccionari.hpp"
#include "generador.hpp"

#include "cronometre.hpp"
#include "display_tabular.hpp"
#include "file_io.hpp"
#include "string_cast.hpp"

using namespace std;
using namespace diccionari;
using namespace utils;

namespace tests_diccionari {
	int test1(int argc, char** argv) {
		cout << "TEST DICCIONARIS 1" << endl;

		int d_size = 5;
		int t_size = 5 * d_size;
		float t_prop = 0.1F;
		Diccionari* dicc;
		vector<paraula> vdicc = vector<paraula>();
		vector<paraula> entrada = vector<paraula>();

		motorAleatori.llavor(time(NULL));
		vdicc = genera_diccionari(d_size);
		dicc = diccionari::factory(2, vdicc);
		entrada = genera_text(t_size, t_prop, vdicc);

		for (paraula i : entrada)
			cout << i << " " << (dicc->existeix(i) ? "FOUND" : "NOT FOUND") << endl;

		cout << endl;
		delete dicc;
		return 0;
	}

	int test2(int argc, char** argv) {
		cout << "TEST DICCIONARIS 2" << endl;

		int d_size = 10000;
		int t_size = 20 * d_size;
		float t_prop = 0.01F;
		Diccionari* dicc;
		vector<paraula> vdicc = vector<paraula>();
		vector<paraula> entrada = vector<paraula>();
		vector<bool> trobats = vector<bool>();

		cout << "Generating ..." << endl;

		motorAleatori.llavor(time(NULL));
		vdicc = genera_diccionari(d_size);
		dicc = diccionari::factory(2, vdicc);
		entrada = genera_text(t_size, t_prop, vdicc);
		trobats.reserve(entrada.size()); // Assegura temps constant en .push_back
		if (dicc == NULL) return -1;

		cout << "Running ..." << endl;

		function<void(void)> cerca_sequencial = function<void(void)>([entrada, &trobats, dicc](void) {
			for (paraula p : entrada)
				trobats.push_back(dicc->existeix(p));
		});

		auto cr1 = crea_Cronometre(cerca_sequencial);
		cr1();
		cout << "Temps sequencial: " << cr1.elapsed<chrono::milliseconds>() << " ms" << endl;

		auto cr2 = Cronometre<void>::crea([&entrada, dicc, &trobats](void) {
			sort(entrada.begin(), entrada.end());
			trobats = dicc->existeix_lot(entrada);
		});
		cr2();
		cout << "Temps lot: " << cr2.elapsed<chrono::milliseconds>() << " ms" << endl;

		cout << endl;
		delete dicc;
		return 0;
	}
        
    int hash_Search() {
		return 0;
    }
}

namespace tests_utils {
	int suma(int a, int b) { return a + b; }

	int cronometre(int argc, char** argv) {
		cout << "TEST CRONOMETRE" << endl;

		auto crono_suma = crea_Cronometre(suma);
		int a = 2, b = 5;
		int suma = crono_suma(a, b);
		cout << a << " + " << b << " = " << suma << endl;
		cout << "Elapsed: " << crono_suma.elapsed<chrono::nanoseconds>() << " ns" << endl;

		cout << endl;
		return 0;
	}

	int disp_t(int argc, char** argv) {
		cout << "TEST DISPLAY_TABULAR" << endl;

		vector<char> v1 = { 'a', 'b', 'c' };
		vector<int>  v2 = { 1, 2, 3, 4 };

		cout << to_string(display_tabular("\t", v1, v2)) << endl;
		cout << to_string(display_tabular_it("\t", v1.begin(), v1.end(), v2.begin(), v2.end())) << endl;

		return 0;
	}
}

int main(int argc, char** argv) {    
	// if (tests_diccionari::test1(argc, argv) < 0) return -1;
	// if (tests_diccionari::test2(argc, argv) < 0) return -2;
	// if (tests_utils::cronometre(argc, argv) < 0) return -3;
	// if (tests_utils::disp_t    (argc, argv) < 0) return -4;

#ifdef _MSC_VER
	cin.ignore();
#endif

	return 0;
}