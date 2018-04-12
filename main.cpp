/* 
 * File:   main.cpp
 * Author: albert.carreira
 *
 * Created on April 12, 2018, 3:41 PM
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h> 
#include <sstream>
using namespace std;

void generador(int n){
  ofstream archivo1;
  archivo1.open ("archivo1");
  
  // Generamos los números aleatorios
  int seed = 1234;
  
  /* Numeros hasta 10 cifras
  long long int tamanoMax = 1000000000000000;
  srand(seed);
  for (int i = 0; i < n; ++i){
        long long int number = rand()%tamanoMax;
        o << number;
        archivo1 << o.str();
        archivo1 << endl;
        o.clear();
        o.str("");
  }*/
  long long int tamanoMax = 100000000000000000000;
  srand(seed);
  for (int i = 0; i < n; ++i){
        ostringstream o;
        long long int number1 = rand();
        long long int number2 = rand();
        long long int number = (number1 * number2)%tamanoMax;
        o << number;
        archivo1 << o.str();
        archivo1 << endl;
        o.clear();
        o.str("");
  }
  
  
  archivo1.close();
}

void reader(int n){
    ifstream archivo1 ("archivo1", ifstream::in);
    vector<string> listaNumeros = vector<string>(n); //Pasar los numeros a int?
    int i = 0;
    
    if (archivo1.is_open()){
        string numero;
        while(getline(archivo1, numero)){
            listaNumeros[i] = numero;
            cout << numero << endl;
        }
        archivo1.close();
    }
    else cout << "No se pudo abrir el archivo: archivo1" << endl; 
    
    
    
}


int main(int argc, char** argv) {
    
    int n = 10;
    generador(n);
    reader(n);

    
    return 0;
}

