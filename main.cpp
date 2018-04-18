#include <iostream>
#include <string>
#include <vector>

#include "diccionari.hpp"

using namespace std;
using namespace diccionari;

int main(int argc, char** argv) {
	int n = 10;
	int seed = 1234;
	string filename = "archivo1.txt";
	vector<paraula> listNum = vector<paraula>();

	generate(filename, n, seed);
	read(filename, listNum);

	for (int i : listNum)
		cout << i << endl;

	cin.ignore();
	return 0;
}

