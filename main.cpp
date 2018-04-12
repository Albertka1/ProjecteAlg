/* 
 * File:   main.cpp
 * Author: albert.carreira
 *
 * Created on April 12, 2018, 3:41 PM
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

void generador(int n){
  ofstream archivo1;
  archivo1.open ("archivo1");
  for (int i = 1; i <= n; ++i){
      ostringstream o;
      o << i;
      archivo1 << o.str();
      archivo1 << endl;
  }

  archivo1.close();
}

void reader(string file){
    ifstream archivo1 (file);
    if (archivo1.is_open()){
        string numero;
        while(getline(archivo1, numero)){
            cout << numero << endl;
        }
        archivo1.close();
    }
    else cout << "No se pudo abrir el archivo: " + file << endl; 
    
    
    vector<int> listaNumeros = vector<int>();
}


int main(int argc, char** argv) {
    
    generador(123);
    
    
    return 0;
}

