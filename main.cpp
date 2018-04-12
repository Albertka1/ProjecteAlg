/* 
* File:   main.cpp
* Author: albert.carreira
*
* Created on April 12, 2018, 3:41 PM
*/

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void generador(int n){
	ofstream fs;
	fs.open ("archivo1.txt");
	for (int i = 1; i <= n; ++i){
		fs << to_string(i);
		fs << endl;
	}

	fs.close();
}

void reader(string filename){
	ifstream fs(filename);
	if (fs.is_open()){
		string numero;
		while(getline(fs, numero)){
			cout << numero << endl;
		}
		fs.close();
	}
	else cout << "No se pudo abrir el archivo: " + filename << endl;
    
    
	vector<int> listaNumeros = vector<int>();
}


int main(int argc, char** argv) {
    
	generador(123);
    
    
	return 0;
}
