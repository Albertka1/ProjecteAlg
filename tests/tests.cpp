#include <algorithm>
#include <chrono>
#include <ctime>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "diccionari.hpp"
#include "generador.hpp"

using namespace std;
using namespace diccionari;

typedef chrono::duration<double, milli> span_s;
typedef chrono::high_resolution_clock hr_clock;

namespace tests {
	template <class R, class ... Args>
	class cronometre {
	private:
		function<R(Args ...)> f;
		double t;
	public:
		cronometre(function<R(Args ...)> func) { t = 0; f = func; }
		R operator()(Args ... args) {
			hr_clock::time_point ini, fi;
			R result;

			ini = hr_clock::now();
			result = f(args ...);
			fi  = hr_clock::now();

			t = span_s(chrono::duration_cast<span_s>(fi - ini)).count();

			return result;
		}
		double elapsed() const { return t; }
	};

	template <class ... Args>
	class cronometre<void, Args ...> {
	private:
		function<void(Args ...)> f;
		double t;
	public:
		cronometre(function<void(Args ...)> func) { t = 0; f = func; }
		void operator()(Args ... args) {
			hr_clock::time_point ini, fi;

			ini = hr_clock::now();
			f(args ...);
			fi = hr_clock::now();

			t = span_s(chrono::duration_cast<span_s>(fi - ini)).count();
		}
		double elapsed() const { return t; }
	};

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
		vector<bool> trobats = vector<bool>(0);
		hr_clock::time_point ini, fi;

		motorAleatori.llavor(time(NULL));
		genera_diccionari(file_dicc, n); // Genera fitxer diccionari
		genera_text(file_test, file_dicc, n * 4); // Genera fitxer test amb 4 vegades n

		dicc = diccionari::factory(2, file_dicc);
		if (dicc == NULL) return -1;

		if (llegeix(file_test, nombres) == -1)
			return -1;
		trobats.reserve(nombres.size()); // Assegura temps constant en .push_back

		ini = hr_clock::now();
		for (paraula p : nombres)
			trobats.push_back(dicc->existeix(p));
		fi = hr_clock::now();
		cout << "Temps sequencial: " << span_s(chrono::duration_cast<span_s>(fi - ini)).count() << " ms" << endl;

		cronometre<void> cr = cronometre<void>([&nombres, dicc, &trobats]() {
			sort(nombres.begin(), nombres.end());
			trobats = dicc->existeix_lot(nombres);
		});
		cr();
		cout << "Temps lot: " << cr.elapsed() << " ms" << endl;

		for (unsigned i = 0; i < nombres.size(); ++i);

#ifdef _MSC_VER
		cin.ignore();
#endif

		delete dicc;
		return 0;
	}
        
        int hash_Search(){
            
        }
}

int main(int argc, char** argv) {    
    return tests::test2(argc, argv); 
}