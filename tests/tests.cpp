#include <algorithm>
#include <iostream>
#include <string>
#include <ctime>
#include <vector>

#include "diccionari.hpp"
#include "generador.hpp"

using namespace std;
using namespace diccionari;

namespace tests {
	int test1(int argc, char** argv) {
		int n = 5;
		string file_dicc = "1_dicc.txt";
		string file_test = "1_test.txt";
		vector<paraula> nombres = vector<paraula>();
		Diccionari* dicc;

		motorAleatori.llavor(time(NULL));
		genera_diccionari(file_dicc, n);
		genera_diccionari(file_test, n * 4);

		dicc = diccionari::factory(1, file_dicc);
		if (dicc == NULL) return -1;

		llegeix(file_test, nombres);
		for (paraula i : nombres)
			cout << i << " " << (dicc->existeix(i) ? "FOUND" : "NOT FOUND") << endl;

#ifdef _MSC_VER
		cin.ignore();
#endif

		delete dicc;
		return 0;
	}

	int test2(int argc, char** argv) {
		int n = 5;
		string file_dicc = "1_dicc.txt";
		string file_test = "1_test.txt";
		vector<paraula> nombres = vector<paraula>();
		Diccionari* dicc;

		motorAleatori.llavor(time(NULL));
		genera_diccionari(file_dicc, n);
		genera_diccionari(file_test, n * 4);

		dicc = diccionari::factory(2, file_dicc);
		if (dicc == NULL) return -1;

		llegeix(file_test, nombres);
		for (paraula i : nombres)
			cout << i << " " << (dicc->existeix(i) ? "FOUND" : "NOT FOUND") << endl;

		cout << endl;

		sort(nombres.begin(), nombres.end());
		vector<bool> trobats = dicc->existeix_lot(nombres);
		for (unsigned i = 0; i < nombres.size(); ++i)
			cout << nombres[i] << " " << (trobats[i] ? "FOUND" : "NOT FOUND") << endl;

#ifdef _MSC_VER
		cin.ignore();
#endif

		delete dicc;
		return 0;
	}
}

int main(int argc, char** argv) { return tests::test2(argc, argv); }