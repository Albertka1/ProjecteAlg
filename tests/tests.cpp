#include <algorithm>
#include <ctime>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "diccionari/diccionari.hpp"
#include "diccionari/factory.hpp"
#include "diccionari/generador.hpp"
#include "diccionari/search_tree.hpp"

#include "utils/cronometre.hpp"
#include "utils/display_tabular.hpp"
#include "utils/file_io.hpp"
#include "utils/string_cast.hpp"

using namespace std;
using namespace diccionari;
using namespace utils;

namespace tests_diccionari {
	int minitest(int n) {
		cout << "\tMINITEST Diccionari(" << n << ")" << endl;

		vector<paraula> vdicc = { 2, 4, 6, 8, 10, 1, 13, 15 };
		vector<bool> trobats = vector<bool>();
		Diccionari *dicc = diccionari::factory(n, vdicc);
		if (dicc == NULL) return -1;

		cout << "Diccionari = " << string(*dicc) << endl;

		cout << "existeix(3): " << to_string(dicc->existeix(3)) << endl;
		cout << "existeix(8): " << to_string(dicc->existeix(8)) << endl;

		trobats = dicc->existeix_lot({ 3, 8 });
		cout << "existeix_lot(3): " << to_string<bool>(trobats[0]) << endl;
		cout << "existeix_lot(8): " << to_string<bool>(trobats[1]) << endl;

		delete dicc;
		cout << endl;
		return 0;
	}

	int existeix(int n, int d, int t, float p) {
		cout << "\tTEST Diccionari(" << n << ")->existeix" << endl;
		cout << "Tamany diccionari:\t" << d << endl;
		cout << "Tamany test:\t\t" << d*t << endl;
		cout << "Proporcio elements:\t" << p << endl;
		
		Diccionari* dicc;
		vector<paraula> vdicc = vector<paraula>();
		vector<paraula> entrada = vector<paraula>();

		motorAleatori.llavor(time(NULL));
		vdicc = genera_diccionari(d);
		dicc = diccionari::factory(n, vdicc);
		entrada = genera_text(d*t, p, vdicc);

		for (paraula par : entrada)
			cout << par << " " << (dicc->existeix(par) ? "FOUND" : "NOT FOUND") << endl;

		cout << endl;
		delete dicc;
		return 0;
	}

	int existeix_lot(int n, int d, int t, float p) {
		cout << "\tTEST Diccionari(" << n << ")->existeix_lot" << endl;
		cout << "Tamany diccionari:\t" << d << endl;
		cout << "Tamany test:\t\t" << d * t << endl;
		cout << "Proporcio elements:\t" << p << endl;

		Diccionari* dicc;
		vector<paraula> vdicc = vector<paraula>();
		vector<paraula> entrada = vector<paraula>();
		vector<bool> trobats = vector<bool>();

		motorAleatori.llavor(time(NULL));
		vdicc = genera_diccionari(d);
		dicc = diccionari::factory(n, vdicc);
		entrada = genera_text(d*t, p, vdicc);

		trobats = dicc->existeix_lot(entrada);

		for (unsigned i = 0; i < entrada.size(); ++i)
			cout << entrada[i] << " " << (trobats[i] ? "FOUND" : "NOT FOUND") << endl;

		cout << endl;
		delete dicc;
		return 0;
	}

	int comparativa(int n, int d, int t, float p) {
		cout << "\tCOMPARATIVA Diccionari(" << n << ")" << endl;
		cout << "Tamany diccionari:\t" << d << endl;
		cout << "Tamany test:\t\t" << d*t << endl;
		cout << "Proporcio elements:\t" << p << endl;
		cout << endl;
		
		Diccionari* dicc;
		vector<paraula> vdicc = vector<paraula>();
		vector<paraula> entrada = vector<paraula>();
		vector<bool> trobats = vector<bool>();

		// cout << "Generating ..." << endl;

		motorAleatori.llavor(time(NULL));
		vdicc = genera_diccionari(d);
		dicc = diccionari::factory(n, vdicc);
		entrada = genera_text(d*t, p, vdicc);
		trobats.reserve(entrada.size()); // Assegura temps constant en .push_back
		if (dicc == NULL) return -1;

		// cout << "Running ..." << endl;

		function<void(void)> cerca_sequencial = function<void(void)>([entrada, &trobats, dicc](void) {
			for (paraula par : entrada)
				trobats.push_back(dicc->existeix(par));
		});

		auto cr1 = crea_Cronometre(cerca_sequencial);
		cr1();
		if (cr1.elapsed<chrono::milliseconds>() > 100LL)
			cout << "Temps sequencial:\t" << cr1.elapsed<chrono::milliseconds>() << " ms" << endl;
		else
			cout << "Temps sequencial:\t" << cr1.elapsed<chrono::microseconds>() << " ys" << endl;
		cout << "Num Comparacions:\t" << dicc->count_comps() << endl;
		dicc->restart_count();

		cout << endl;

		auto cr2 = Cronometre<void>([&entrada, dicc, &trobats](void) {
			sort(entrada.begin(), entrada.end());
			trobats = dicc->existeix_lot(entrada);
		});
		cr2();
		if (cr2.elapsed<chrono::milliseconds>() > 100LL)
			cout << "Temps lot:\t\t" << cr2.elapsed<chrono::milliseconds>() << " ms" << endl;
		else
			cout << "Temps lot:\t\t" << cr2.elapsed<chrono::microseconds>() << " ys" << endl;
		cout << "Num Comparacions:\t" << dicc->count_comps() << endl;
		dicc->restart_count();

		cout << endl;
		delete dicc;
		return 0;
	}
}

namespace tests_utils {
	int suma(int a, int b) { return a + b; }

	int cronometre() {
		cout << "\tTEST CRONOMETRE" << endl;

		auto crono_suma = crea_Cronometre(suma);
		int a = 2, b = 5;
		int suma = crono_suma(a, b);
		cout << a << " + " << b << " = " << suma << endl;
		cout << "Elapsed: " << crono_suma.elapsed<chrono::nanoseconds>() << " ns" << endl;

		cout << endl;
		return 0;
	}

	int disp_t() {
		cout << "\tTEST DISPLAY_TABULAR" << endl;

		vector<char> v1 = { 'a', 'b', 'c' };
		vector<int>  v2 = { 1, 2, 3, 4 };

		cout << to_string(display_tabular("\t", v1, v2)) << endl;
		cout << to_string(display_tabular_it("\t", v1.begin(), v1.end(), v2.begin(), v2.end())) << endl;

		return 0;
	}
}

int main(int argc, char** argv) {
	int i = -1;

	//if (tests_diccionari::minitest(DictType::tCercaSequencial) < 0) return i; --i;
	//if (tests_diccionari::existeix(DictType::tCercaSequencial, 8, 5, 0.1f) < 0) return i; --i;
	//if (tests_diccionari::existeix_lot(DictType::tCercaSequencial, 8, 5, 0.1f) < 0) return i; --i;

	//if (tests_diccionari::minitest(DictType::tSetFind) < 0) return i; --i;
	//if (tests_diccionari::existeix(DictType::tSetFind, 8, 5, 0.1f) < 0) return i; --i;
	//if (tests_diccionari::existeix_lot(DictType::tSetFind, 8, 5, 0.1f) < 0) return i; --i;

	//if (tests_diccionari::minitest(DictType::tUSetFind) < 0) return i; --i;
	//if (tests_diccionari::existeix(DictType::tUSetFind, 8, 5, 0.1f) < 0) return i; --i;
	//if (tests_diccionari::existeix_lot(DictType::tUSetFind, 8, 5, 0.1f) < 0) return i; --i;

	//if (tests_diccionari::minitest(DictType::tBTree) < 0) return i; --i;
	//if (tests_diccionari::existeix(DictType::tBTree, 8, 5, 0.1f) < 0) return i; --i;
	//if (tests_diccionari::existeix_lot(DictType::tBTree, 8, 5, 0.1f) < 0) return i; --i;

	if (tests_diccionari::minitest(DictType::tBinarySearchTree) < 0) return i; --i;
	if (tests_diccionari::existeix(DictType::tBinarySearchTree, 8, 5, 0.1f) < 0) return i; --i;
	if (tests_diccionari::existeix_lot(DictType::tBinarySearchTree, 8, 5, 0.1f) < 0) return i; --i;

	//if (tests_diccionari::minitest(DictType::tTreap) < 0) return i; --i;
	//if (tests_diccionari::existeix(DictType::tTreap, 8, 5, 0.1f) < 0) return i; --i;
	//if (tests_diccionari::existeix_lot(DictType::tTreap, 8, 5, 0.1f) < 0) return i; --i;

	//if (tests_diccionari::comparativa(DictType::tSetFind, 1000, 100, 0.01f) < 0) return i; --i;
	//if (tests_diccionari::comparativa(DictType::tSetFind, 1000, 100, 0.05f) < 0) return i; --i;
	//if (tests_diccionari::comparativa(DictType::tSetFind, 30000, 100, 0.01f) < 0) return i; --i;
	//if (tests_diccionari::comparativa(DictType::tSetFind, 30000, 100, 0.05f) < 0) return i; --i;

	//if (tests_utils::cronometre() < 0) return i; --i;
	//if (tests_utils::disp_t    () < 0) return i; --i;

#ifdef _MSC_VER
	cin.ignore();
#endif

	return 0;
}
