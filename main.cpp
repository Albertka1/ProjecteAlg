#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h> 
#include <string>
#include <vector>

using namespace std;

typedef unsigned long long int u2long;

// Genera n nombres aleatoris amb llavor seed i els desa al arxiu especificat per filename
void generador(string filename, int n, int seed) {
	ofstream fs(filename);
	u2long tamanoMax = 10000000000000000000;
	std::default_random_engine generator(seed);
	std::uniform_int_distribution<u2long> distribution(0, tamanoMax-1);

	if (!fs.is_open()) {
		cerr << "No se pudo abrir el archivo: " + filename << endl;
		return;
	}
	// Generamos los números aleatorios
	for (int i = 0; i < n; ++i) { 
		u2long rand = distribution(generator);
		fs << to_string(rand);
		fs << endl;
	}

	fs.close();
}

// Llegeix els nombres de l'arxiu filename i els desa en listNum
void reader(string filename, vector<u2long>& listNum) {
	ifstream fs(filename);
	string num;

	if (!fs.is_open()) {
		cerr << "No se pudo abrir el archivo: " + filename << endl;
		return;
	}

	while (getline(fs, num))
		listNum.push_back(stoll(num));

	fs.close();
}


int main(int argc, char** argv) {
	int n = 10;
	int seed = 1234;
	string filename = "archivo1.txt";
	vector<u2long> listNum = vector<u2long>();

	generador(filename, n, seed);
	reader(filename, listNum);

	for (int i : listNum)
		cout << i << endl;

	cin.ignore();
	return 0;
}

