#include <algorithm>
#include <ctime>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "diccionari/diccionari.hpp"
#include "diccionari/factory.hpp"
#include "diccionari/generador.hpp"
#include "utils/cronometre.hpp"
#include "utils/display_tabular.hpp"
#include "utils/file_io.hpp"
#include "utils/string_cast.hpp"

using namespace std;
using namespace diccionari;
using namespace utils;

namespace tests_diccionari {
	template <class ... Args>
	int minitest(int n) {
		cerr << "MINITEST Diccionari(" << n << ")" << endl;
		cout << "\tMINITEST Diccionari(" << n << ")" << endl;
		
		vector<paraula> vdicc = { 2, 4, 6, 8, 10, 1, 13, 15 };
		vector<bool> trobats = vector<bool>();
		Diccionari *dicc = diccionari::factory(n, vdicc, args ...);
		if (dicc == NULL) return -1;
		
		cout << "Diccionari = " << string(*dicc) << endl;
		
		cout << "existeix(3): " << to_string(dicc->existeix(3)) << endl;
		cout << "existeix(8): " << to_string(dicc->existeix(8)) << endl;
		
		vector<paraula> lot = { 3, 8 };
		trobats = dicc->existeix_lot(lot);
		cout << "Optimitza existeix_lot: " << (dicc->optimitza_lot() ? "SI" : "NO") << endl;
		cout << "existeix_lot(3): " << to_string<bool>(trobats[0]) << endl;
		cout << "existeix_lot(8): " << to_string<bool>(trobats[1]) << endl;
		
		trobats = dicc->existeix_lot_ordenat(lot);
		cout << "Optimitza existeix_lot_ordenat: " << (dicc->optimitza_lot_ordenat() ? "SI" : "NO") << endl;
		cout << "existeix_lot(3): " << to_string<bool>(trobats[0]) << endl;
		cout << "existeix_lot(8): " << to_string<bool>(trobats[1]) << endl;
		
		delete dicc;
		cout << endl;
		return 0;
	}

	template <class ... Args>
	int existeix(int n, int d, int t, float p, Args ... args) {
		cerr << "TEST Diccionari(" << n << ")->existeix" << endl;
		cout << "\tTEST Diccionari(" << n << ")->existeix" << endl;
		cout << "Tamany diccionari:\t" << d << endl;
		cout << "Tamany test:\t\t" << d*t << endl;
		cout << "Proporcio elements:\t" << p << endl;
		
		Diccionari* dicc;
		vector<paraula> vdicc = vector<paraula>();
		vector<paraula> entrada = vector<paraula>();
		
		motorAleatori.llavor(time(NULL));
		vdicc = genera_diccionari(d);
		dicc = diccionari::factory(n, vdicc, args ...);
		entrada = genera_text(d*t, p, vdicc);
		
		for (paraula par : entrada)
			cout << par << "\t" << (dicc->existeix(par) ? "FOUND" : "NOT FOUND") << endl;
		
		cout << endl;
		delete dicc;
		return 0;
	}

	template <class ... Args>
	int existeix_lot(int n, int d, int t, float p, Args ... args) {
		cerr << "TEST Diccionari(" << n << ")->existeix_lot" << endl;
		cout << "\tTEST Diccionari(" << n << ")->existeix_lot" << endl;
		cout << "Tamany diccionari:\t" << d << endl;
		cout << "Tamany test:\t\t" << d * t << endl;
		cout << "Proporcio elements:\t" << p << endl;
		
		Diccionari* dicc;
		vector<paraula> vdicc = vector<paraula>();
		vector<paraula> entrada = vector<paraula>();
		vector<bool> trobats = vector<bool>();
		
		dicc = diccionari::factory(n, {});
		if (!dicc->optimitza_lot_ordenat()) {
			cout << "OPERACIO NO OPTIMITZADA" << endl << endl;
			return 0;
		}
		delete dicc;
		
		motorAleatori.llavor(time(NULL));
		vdicc = genera_diccionari(d);
		dicc = diccionari::factory(n, vdicc, args ...);
		entrada = genera_text(d*t, p, vdicc);
		
		trobats = dicc->existeix_lot(entrada);
		
		for (unsigned i = 0; i < entrada.size(); ++i)
			cout << entrada[i] << "\t" << (trobats[i] ? "FOUND" : "NOT FOUND") << endl;
		
		cout << endl;
		delete dicc;
		return 0;
	}
	
	template <class ... Args>
	int existeix_lot_ordenat(int n, int d, int t, float p, Args ... args) {
		cerr << "TEST Diccionari(" << n << ")->existeix_lot_ordenat" << endl;
		cout << "\tTEST Diccionari(" << n << ")->existeix_lot_ordenat" << endl;
		cout << "Tamany diccionari:\t" << d << endl;
		cout << "Tamany test:\t\t" << d * t << endl;
		cout << "Proporcio elements:\t" << p << endl;
		
		Diccionari* dicc;
		vector<paraula> vdicc = vector<paraula>();
		vector<paraula> entrada = vector<paraula>();
		vector<bool> trobats = vector<bool>();
		
		dicc = diccionari::factory(n, {});
		if (!dicc->optimitza_lot_ordenat()) {
			cout << "OPERACIO NO OPTIMITZADA" << endl << endl;
			return 0;
		}
		delete dicc;
		
		motorAleatori.llavor(time(NULL));
		vdicc = genera_diccionari(d);
		dicc = diccionari::factory(n, vdicc, args ...);
		entrada = genera_text(d*t, p, vdicc);
		
		trobats = dicc->existeix_lot_ordenat(entrada);
		
		for (unsigned i = 0; i < entrada.size(); ++i)
			cout << entrada[i] << "\t" << (trobats[i] ? "FOUND" : "NOT FOUND") << endl;
		
		cout << endl;
		delete dicc;
		return 0;
	}

	int count(const vector<bool>& vb) {
		int i = 0;
		for (bool b : vb) if (b) ++i;
		return i;
	}
	
	template <class ... Args>
	int comparativa(int n, int d, int t, float p, bool print, Args ... args) {
		cerr << "COMPARATIVA Diccionari(" << n << ")" << endl;
		cout << "\tCOMPARATIVA Diccionari(" << n << ")" << endl;
		cout << "Tamany diccionari:\t" << d << endl;
		cout << "Tamany test:\t\t" << d*t << endl;
		cout << "Proporcio elements:\t" << p << endl;
		cout << endl;
		
		Diccionari* dicc;
		vector<paraula> vdicc = vector<paraula>();
		vector<paraula> entrada = vector<paraula>();
		vector<bool> t1 = vector<bool>(), t2 = vector<bool>(), t3 = vector<bool>();
		
		// cout << "Generating ..." << endl;
		
		motorAleatori.llavor(time(NULL));
		vdicc = genera_diccionari(d);
		entrada = genera_text(d*t, p, vdicc);
		vector<paraula> copia = vector<paraula>(entrada);
		t1.reserve(entrada.size()); // Assegura temps constant en .push_back
		
		auto cr0 = crea_Cronometre(function<void(void)>(
		[n, vdicc, &dicc](void) {
				dicc = diccionari::factory(n, vdicc, args ...);
			}
		));
		cr0();
		if (dicc == NULL) return -1;
		if (cr0.elapsed<chrono::milliseconds>() > 100LL)
			cout << "Temps creacio:\t\t" << cr0.elapsed<chrono::milliseconds>() << " ms" << endl;
		else
			cout << "Temps creacio:\t\t" << cr0.elapsed<chrono::microseconds>() << " ys" << endl;
		cout << "Load Factor:\t\t" << dicc->getLoadFactor() << endl;
		cout << endl;
		
		// cout << "Running ..." << endl;
		
		function<void(void)> cerca_sequencial = function<void(void)>([dicc, entrada, &t1](void) {
			for (paraula par : entrada)
				t1.push_back(dicc->existeix(par));
		});
		
		auto cr1 = crea_Cronometre(cerca_sequencial);
		cr1();
		if (cr1.elapsed<chrono::milliseconds>() > 100LL)
			cout << "Temps sequencial:\t" << cr1.elapsed<chrono::milliseconds>() << " ms" << endl;
		else
			cout << "Temps sequencial:\t" << cr1.elapsed<chrono::microseconds>() << " ys" << endl;
		cout.imbue(locale("")); cout << "Num Comparacions cerca:\t" << dicc->count_comps() << endl;
		dicc->restart_count();
		cout << "Load Factor: " << dicc->getLoadFactor() << endl;
		cout << endl;
		vector<pair<paraula, bool>> pt1 = vector<pair<paraula, bool>>(0);
		for (unsigned i = 0; i < t1.size(); ++i)
			pt1.push_back(make_pair(entrada[i], t1[i]));
		sort(pt1.begin(), pt1.end(), [](pair<paraula, bool> a, pair<paraula, bool> b) {return a < b; });
		auto it1 = unique(pt1.begin(), pt1.end(), [](pair<paraula, bool> a, pair<paraula, bool> b) {return a == b; });
		pt1.resize(it1 - pt1.begin());
		t1.resize(0);
		for (auto p : pt1)
			t1.push_back(p.second);

		auto cr2 = Cronometre<void>([dicc, &entrada, &t2](void) {
			t2 = dicc->existeix_lot(entrada);
		});
		cr2();
		if (cr2.elapsed<chrono::milliseconds>() > 100LL)
			cout << "Temps lot:\t\t" << cr2.elapsed<chrono::milliseconds>() << " ms" << endl;
		else
			cout << "Temps lot:\t\t" << cr2.elapsed<chrono::microseconds>() << " ys" << endl;
		cout << "Num Comparacions:\t" << dicc->count_comps() << endl;
		dicc->restart_count();

		cout << endl;

		auto cr3 = Cronometre<void>([dicc, entrada, &t3](void) {
			t3 = dicc->existeix_lot_ordenat(entrada);
		});
		cr3();
		if (cr3.elapsed<chrono::milliseconds>() > 100LL)
			cout << "Temps lot ordenat:\t" << cr3.elapsed<chrono::milliseconds>() << " ms" << endl;
		else
			cout << "Temps lot ordenat:\t" << cr3.elapsed<chrono::microseconds>() << " ys" << endl;
		cout << "Num Comparacions:\t" << dicc->count_comps() << endl;
		dicc->restart_count();

		cout << endl;

		cout << "Num trobats" << endl;
		cout << "seq: " << count(t1) << endl;
		cout << "lot: " << count(t2) << endl;
		cout << "ord: " << count(t3) << endl;

		cout << endl;

		sort(copia.begin(), copia.end());
		auto it = unique(copia.begin(), copia.end());

		copia.resize(it - copia.begin());
		cout << "Reordenament" << endl;
		bool E_C = entrada == copia;
		cout << (E_C ? "OK" : "ENTRADA ADULTERADA") << endl;
		
		if (print) {
			cout << endl;
			if (!E_C)
				cout << to_string(display_tabular("\t\t", entrada, copia)) << endl;
			else
				cout << to_string(display_tabular("\t\t", copia, t1, t2, t3)) << endl;
		}
		
		cout << endl;
		delete dicc;
		return 0;
	}

	template <class ... Args>
	int print(int n, int d, Args ... args) {
		cerr << "PRINT Diccionari(" << n << ")" << endl;
		cout << "\tPRINT Diccionari(" << n << ")" << endl;
		cout << "Tamany diccionari:\t" << d << endl;
		cout << endl;

		motorAleatori.llavor(time(NULL));
		Diccionari *dicc = factory(n, genera_diccionari(d));
		if (dicc == NULL) return -1;
		cout << (string)*dicc << endl;
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

template <class ... Args>
int test_funcs(int type, int d, int t, float p, Args ... args) {
	int i = -1;
	
	if (tests_diccionari::minitest(type, args ...) < 0) return i; --i;
	if (tests_diccionari::existeix(type, d, t, p, args ...) < 0) return i; --i;
	if (tests_diccionari::existeix_lot(type, d, t, p, args ...) < 0) return i; --i;
	if (tests_diccionari::existeix_lot_ordenat(type, d, t, p, args ...) < 0) return i; --i;
	
	return 0;
}

template <class ... Args>
int metrics(int type, int d, int t, float p, bool print, Args ... args) {
	int i = -1;
	
	if (tests_diccionari::comparativa(type, d, t, p, print) < 0) return i; --i;
	
	return 0;
}

template <class ... Args>
int multimetrics(const vector<int>& types, const vector<int>& ds, const vector<int>& ts, const vector<float>& fs, bool print, Args ... args) {
	int i = -1;

	for (int type : types)
	{
		for (int d : ds) for (int t : ts) for (float f : fs) {
			if (metrics(type, d, t, f, print) < 0)
				return i;
			--i;
		}

		cout << "-------------------------------------" << endl << endl;
	}

	return 0;
}

int prova() {
	int i = -1;
	
	// if (test_funcs(DictType::tCercaSequencial, 8, 5, 0.1f) < 0) return i; --i;
	// if (test_funcs(DictType::tSetFind, 8, 5, 0.1f) < 0) return i; --i;
	// if (test_funcs(DictType::tUSetFind, 8, 5, 0.1f) < 0) return i; --i;
	// if (test_funcs(DictType::tBTree, 8, 5, 0.1f) < 0) return i; --i;
	// if (test_funcs(DictType::tBST, 8, 5, 0.1f) < 0) return i; --i;
	// if (test_funcs(DictType::tTreap, 8, 5, 0.1f) < 0) return i; --i;
	
	/*if (
		multimetrics(
			{
				DictType::tCercaSequencial,
				DictType::tSetFind,
				DictType::tUSetFind,
				DictType::tBST,
				DictType::tTreap,
			},
			{
				1000,
				100000,
			},
			{
				10,
				100,
			},
			{
				0.01f,
				0.05f,
			},
			false
		) < 0) return i; --i;*/

	// if (tests_diccionari::print(DictType::tTreap, 20) < 0) return i; --i;
	
	// if (tests_utils::cronometre() < 0) return i; --i;
	// if (tests_utils::disp_t    () < 0) return i; --i;

#ifdef _MSC_VER
	cin.ignore();
#endif
	
	return 0;
}

template <class ... Args>
int analitza(int type, int d, int t, float p, Args ... args) {
	Diccionari *dicc;
	int dt = d * t;
	vector<paraula> vdicc, vtext;
	vector<bool> troba = vector<bool>(dt, false);

	vdicc = genera_diccionari(d);
	vtext = genera_text(dt, p, vdicc);

	cout << "Tipus: " << type << endl;
	cout << "d: " << d << "\tt: " << t << "\tp: " << p << endl;
	cout << endl;
	
	auto c_crea = Cronometre<void>([&dicc, type, vdicc, args ...]() {
		dicc = factory(type, vdicc, args ...);
	});
	c_crea();

	if (dicc == NULL) return -1;

	cout << "CREACIO" << endl;
	cout << "temps: " << to_string(c_crea) << endl;
	cout << "n comps: " << dicc->count_comps() << endl;
	cout << "load: " << dicc->getLoadFactor() << endl;
	cout << endl;

	dicc->restart_count();

	auto c_seq = Cronometre<void>([&troba, dicc, vtext, dt]() {
		for (int i = 0; i < dt; ++i)
			troba[i] = dicc->existeix(vtext[i]);
	});
	c_seq();

	cout << "CERCA SEQ" << endl;
	cout << "temps: " << to_string(c_seq) << endl;
	cout << "n comps: " << dicc->count_comps() << endl;
	cout << endl;

	if (dicc->optimitza_lot()) {
		// Reiniciem diccionari i text per evitar cache
		vdicc = genera_diccionari(d);
		vtext = genera_text(dt, p, vdicc);
		dicc = factory(type, vdicc, args ...);

		if (dicc == NULL) return -2;
		
		auto c_lot = Cronometre<void>([&troba, dicc, &vtext]() {
			troba = dicc->existeix_lot(vtext);
		});
		c_lot();

		cout << "CERCA LOT" << endl;
		cout << "temps: " << to_string(c_lot) << endl;
		cout << "n comps: " << dicc->count_comps() << endl;
		cout << endl;
	}

	if (dicc->optimitza_lot_ordenat()) {
		// Reiniciem diccionari i text per evitar cache
		vdicc = genera_diccionari(d);
		vtext = genera_text(dt, p, vdicc);
		dicc = factory(type, vdicc, args ...);

		if (dicc == NULL) return -3;
		
		auto c_ord = Cronometre<void>([&troba, dicc, vtext]() {
			troba = dicc->existeix_lot_ordenat(vtext);
		});
		c_ord();

		cout << "CERCA ORD" << endl;
		cout << "temps: " << to_string(c_ord) << endl;
		cout << "n comps: " << dicc->count_comps() << endl;
		cout << endl;
	}

	return 0;
}

int main(int argc, char** argv){
	//prova();
	//analitza(...);

	int type = DictType::tBST;
	for (int d : {10000, 100000}) for (int t : {10, 100}) for (float p : {0.005f, 0.01f, 0.02f, 0.05f, 0.1f})
		for (int i = 0; i < 10; i++) analitza(type, d, t, p);
}
