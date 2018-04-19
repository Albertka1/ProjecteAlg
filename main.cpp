#include <iostream>
#include <string>
#include <ctime>
#include <vector>

#include "diccionari.hpp"

using namespace std;
using namespace diccionari;

int main(int argc, char** argv) {
	int n = 10;
	string file_dicc = "dicc.txt";
	string file_test1 = "test1.txt";
	string file_test2 = "test2.txt";
	vector<paraula> listNum = vector<paraula>();
	Diccionari* dicc;

	randomEngine.seed(time(NULL));
	generate(file_dicc, n);
	generate(file_test1, n*10);
	generate(file_test2, n*10);

	dicc = diccionari::factory(1, file_dicc);
	if (dicc == NULL) return -1;
	
	read(file_test1, listNum);
	for (paraula i : listNum) {
		cout << i << " ";
		cout << (dicc->exists(i) ? "FOUND" : "NOT FOUND") << endl;
	}

	cout << endl;

	read(file_test2, listNum);
	for (paraula i : listNum) {
		cout << i << " ";
		cout << (dicc->exists(i) ? "FOUND" : "NOT FOUND") << endl;
	}

	delete dicc;

	cin.ignore();
	return 0;
}

