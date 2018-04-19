#include <iostream>
#include <string>
#include <ctime>
#include <vector>

#include "diccionari.hpp"
#include "generador.hpp"

using namespace std;
using namespace diccionari;

int main(int argc, char** argv) {
	int n = 10;
	string file_dicc  = "dicc.txt";
	string file_test1 = "test1.txt";
	string file_test2 = "test2.txt";
	vector<paraula> nombres = vector<paraula>();
	Diccionari* dicc;

	motorAleatori.llavor(time(NULL));
	genera_diccionari(file_dicc, n);
	genera_diccionari(file_test1, n*10);
	genera_diccionari(file_test2, n*10);

	dicc = diccionari::factory(1, file_dicc);
	if (dicc == NULL) return -1;
	
	llegeix(file_test1, nombres);
	for (paraula i : nombres)
		cout << i << " " << (dicc->existeix(i) ? "FOUND" : "NOT FOUND") << endl;

	cout << endl;

	llegeix(file_test2, nombres);
	for (paraula i : nombres)
		cout << i << " " << (dicc->existeix(i) ? "FOUND" : "NOT FOUND") << endl;

#ifdef _MSC_VER
	cin.ignore();
#endif

	delete dicc;
	return 0;
}

