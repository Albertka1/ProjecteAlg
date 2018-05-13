#ifndef diccionari_filtres_hpp
#define diccionari_filtres_hpp

#include <vector>
#include <functional>
#include <iterator>
#include <set>
#include <unordered_set>
#include <stdint.h>
//#include <stdio.h>
#include <string.h>
#include <cmath>

//#include <sstream>

#include <iostream>
#include <iomanip>

#include "diccionari.hpp"
#include "MurmurHash3.hpp"
using namespace std;

namespace diccionari {
	class SimpleBloom : public Diccionari { //rapido, no permite deletes, coste espacial bajo
		private:
			std::vector<paraula> pars;
			std::vector<bool> bits;
			//int n;  //tamany del vector pars
			float p = 0.001; // acceptable false positive rating (0.001 -> 0.1%)
			unsigned int n;   //numero de elementos
			unsigned int k;   //numero de hash per paraula
			unsigned int m;   //tamany del vector bits
			
			void setFalses() { bits = std::vector<bool>(m,false); }
				
			unsigned int Stupid_hash(int k, paraula p) const {
				unsigned int sum=0;
				while (p > 0) {
					sum += p%10;
					p /=10;
				}
				sum = sum<<k-1;
				return sum % m;                          
			}
				
				
			unsigned long long Murmur_hash(int k, paraula p) const {
				uint32_t seed = (uint32_t)k;
				uint32_t *hash_otpt[1] = {0};
				
				const unsigned long long *key = &p;
			
				//void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );
				//void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );
				MurmurHash3_x86_32(key, sizeof(paraula), seed, hash_otpt);
				
				unsigned long long res;
				stringstream ss;
				ss << *hash_otpt;
				ss >> hex >>res;
								
				return res%m;
			}
				
			void Simple(const std::vector<paraula>& v, int k) {
				unsigned int kk = k<<2;
				for (paraula p: v) bits[Murmur_hash(kk,p)] = true;
			}
		
		public:
			SimpleBloom (const std::vector<paraula>& v) : Diccionari() { 
				pars = std::vector<paraula>(v);
				n = v.size();
				m = round(-(n*log(p)) / (log(2)*log(2)));  //m > k*n 
				k = ceil(m/n*log(2));
				cout << "el numero N es: " << n << endl << endl;
				cout << "el numero M es: " << m << endl << endl;
				cout << "el numero K es: " << k << endl << endl;
				setFalses();
				for (int i = 0; i < k; i++) Simple(v, i);
			}
			
			macro_stringCast
			bool existeix(paraula p) const {
				bool conte_bool = true;
				for (int i = 0; i < k; i++){
					unsigned int ii = i<<2;
					if (bits[Murmur_hash(ii,p)] == false) {
						conte_bool = false; 
						break;
					}
				}
				return conte_bool; 
			}
	};

	class CountingBloom : public Diccionari { //rapido, permite deletes, coste espacial alto
		private:
			std::vector<paraula> pars;
            std::vector<uint8_t> counters; //valores [0,255]

            float p = 0.001; // acceptable false positive rating (0.001 -> 0.1%)
			unsigned int n;   //numero de elementos
			unsigned int k;   //numero de hash per paraula
			unsigned int m;   //tamany del vector bits
                        
            unsigned long Stupid_hash(int k, paraula p) const {
				unsigned long sum=0;
				while (p > 0) {
					sum += p%10;
					p /=10;
				}
				sum = sum<<k-1;
				return sum % m; 
            }
            
            void setZeros() { counters = std::vector<uint8_t>(m, 0); }
            
            unsigned long long Murmur_hash(int k, paraula p) const {
				uint32_t seed = (uint32_t)k;
				uint32_t *hash_otpt[2] = {0};
				const unsigned long long *key = &p;
				
				//void MurmurHash3_x86_32  ( const void * key, int len, uint32_t seed, void * out );
				//void MurmurHash3_x64_128 ( const void * key, int len, uint32_t seed, void * out );
				MurmurHash3_x86_32(key, sizeof(paraula), seed, hash_otpt);
				//MurmurHash3_x86_32(key2, sizeof(unsigned char), seed, hash_otpt);
				
				unsigned long long res;
				stringstream ss;
				ss << *hash_otpt;
				ss >> hex >>res;
								
				return res%m;
			}
                        
            void Counting(const std::vector<paraula>& v, const int k) {
				int kk = k<<2;
				for (paraula p: v) {
					counters[Murmur_hash(kk,p)]++;
                }
            }
		public:
			CountingBloom(const std::vector<paraula>& v) : Diccionari() { 
				pars = std::vector<paraula>(v);
				n = v.size();
				m = round(-(n*log(p)) / (log(2)*log(2)));  //m > k*n 
				k = ceil(m/n*log(2));   
                setZeros();
                for (int i = 0; i < k; i++) Counting(v, i); 
            }
			
			macro_stringCast
			bool existeix(paraula p) const{
				bool conte = true;
                for (int i = 0; i < k; i++){
					int ii = i<<2;
					if (counters[Murmur_hash(ii,p)] == 0) {
						conte = false; 
						break;
					}
                }
                return conte;
            }
                        
            bool eliminar(paraula p) {
				for (int i = 0; i < k; i++) {
					int ii = i<<2;
					counters[Murmur_hash(ii,p)]--;
				}
			}
			
	};
	
	class Quotient : public Diccionari { //deberia ser rapido (permite deletes, coste espacial alto)
		private:
			struct quotient_slot {
				quotient_slot(): fr(0), is_occupied(false), is_continuation(false), is_shifted(false){}
				unsigned long fr; //remainder
				bool is_occupied;
				bool is_continuation;
				bool is_shifted;
			};
			
			std::vector<paraula> pars;
			std::vector<quotient_slot> quotient;
			unsigned int m;   //tamany del hash table
			unsigned int p = 16;  //fingersprint size
			unsigned int r;  //least significants bits 

			
			//we need a fingersprint f
			//p-bit fingersprint  (qf stores p-bit of element)
			//r least significant bits (remainder)
			//q = p - r most significant bits (quotient)
			//hash table has 2^q slots
			
			//fr = f mod 2^r  (remainder)
			//fq = |_ f/2^r_| (quotient)
			//remainder is stored in the bucket indexed by the quotient
			//each bucket contains 3 bits, all 0 at th begining
			//is_occupied, is_continuation, is_shifted
			
			unsigned long Murmur_hash(paraula par) const {
				uint32_t seed = 1; //95 o 90
				uint32_t *hash_otpt[1] = {0};
				const unsigned long long *key = &par;

				MurmurHash3_x86_32(key, sizeof(paraula), seed, hash_otpt);
				
				unsigned long long res;
				stringstream ss;
				ss << *hash_otpt;
				ss >> hex >>res;
				
				int v = 32-p;
				int murmur_mod = (int)pow(2.0, v);
				
				return (unsigned long)res/murmur_mod;
			}
			
			
			void insert_element(const int index, const unsigned long remainder) {
				int i = index;
				bool inserted = false;
				int cluster_num = 0;
				while (!inserted) {
					cout << "ESTO ES " << i << endl;
					cout << "THIS SLOT: " << quotient[i].fr << endl;
					cout << "MY REMAINDER: " << remainder << endl;
					
					if (!quotient[i].is_occupied and !quotient[i].is_continuation and !quotient[i].is_shifted) { //insert directe al final (cua del run)
						cout << "DIRECT INSERT" << endl;
						inserted = true;
						quotient[i].fr = remainder;
						quotient[i].is_continuation= true;
						quotient[i].is_shifted = true;
					}
					else if (quotient[i].fr > remainder) { //encontramos su posicion y insertamos desplazando los demas
						cout << "INSERTAMOS" << endl;
						inserted = true;
						bool was_first = false;
						bool all_shifted = false;
						quotient_slot aux = quotient[i];
						quotient[i].fr = remainder;
						if (aux.is_occupied) was_first = true;  
						i++;
						while (!all_shifted) {
							cout << "SHIFTAMOS A " << i << endl;
							quotient_slot aux2 = quotient[i];
							quotient[i].fr = aux.fr;
							if ((!aux2.is_occupied and !aux2.is_continuation and !aux2.is_shifted) or (i == m)) all_shifted = true;
							if (was_first) {
								cout << "WAS FIRST" << endl;
								quotient[i].is_continuation = true;
								was_first = false;
							}
							quotient[i].is_shifted = true;
							cout << "fr :" << quotient[i].fr << endl;
							cout << "occ:" << quotient[i].is_occupied << endl;
							cout << "con:" << quotient[i].is_continuation << endl;
							cout << "shi:" << quotient[i].is_shifted << endl;
							cout << endl;
							aux = aux2;
							i++;
						}
					}
					else if (cluster_num == 1 and !quotient[i].is_continuation) {//fin de cluster, insertamos al final y desplazamos si hace falta
						cout << "FIN DE CLUSTER" << endl;
						inserted = true;
						bool all_shifted = false;	
						quotient_slot aux = quotient[i];
						quotient[i].fr = remainder;
						quotient[i].is_continuation = true;
						quotient[i].is_shifted = true;
						i++;
						while (!all_shifted) {
							cout << "SHIFTAMOS2 A " << i << endl;
							quotient_slot aux2 = quotient[i];
							quotient[i].fr = aux.fr;
							if ((!aux2.is_occupied and !aux2.is_continuation and !aux2.is_shifted) or (i == m)) all_shifted = true;
							quotient[i].is_shifted = true;
							aux = aux2;
							i++;
						}
					}
					++i;
					cluster_num = 1;
					cout << endl;
					
				}
			}
			
			void add_element(const unsigned long f) {
				int mod = (int)pow(2.0, r);
				unsigned long remainder = f % mod; //fr
				unsigned long qindex = floor(f / mod); //fq
				
				cout << "mod es: " << mod << endl;
				cout << "remainder es: " << remainder << endl;
				cout << "qindex es: " << qindex << endl << endl;
				
				quotient_slot canonical = quotient[qindex];
				
				//el slot esta vacio, insertamos primer elemento del run en su slot canonico.
				if (!canonical.is_occupied and !canonical.is_continuation and !canonical.is_shifted) { //0;0;0;
					quotient[qindex].is_occupied = true;
					quotient[qindex].fr = remainder;
				}
				
				else if (!canonical.is_occupied and canonical.is_continuation and canonical.is_shifted) { //0;1;1;
					quotient[qindex].is_occupied = true;
					bool inserted = false;
					++qindex;
					while (!inserted and qindex < m) {
						cout << "INDEX Q " << qindex << endl;
						if (!quotient[qindex].is_continuation) {
							inserted = true;
							quotient[qindex].is_shifted = true;
							quotient[qindex].fr = remainder;
						}
						++qindex;
						
					}
					
				}
				
				else if (canonical.is_occupied and !quotient[qindex].is_continuation and !quotient[qindex].is_shifted) { //1;0;0;
						insert_element(qindex, remainder);
				}
				//else if (!)
			}
			
			
		public:
			Quotient(const std::vector<paraula>& v) : Diccionari() {
				//unsigned int q = p-r;
				unsigned int n = v.size();
				unsigned int q = log2(n)+1;
				r = p-q;
				pars = std::vector<paraula>(v);
				m = (unsigned int)pow(2.0,q);
				
				cout << "el numero N es: " << v.size() << endl << endl;
				cout << "el numero P es: " << p << endl << endl;
				cout << "el numero R es: " << r << endl << endl;
				cout << "el numero Q es: " << q << endl << endl;
				cout << "el numero M es: " << m << endl << endl;
				
				quotient = vector<quotient_slot>(m);
				
				cout << endl << endl;
				for (paraula p1: v) {
					unsigned long f = Murmur_hash(p1);
					cout << "la palabra '" << p1 << "' tiene como valor hash: " << f << endl;
					add_element(f);
					cout << endl;
				}
				
				for (int i = 0; i < m; i++) {
					cout << "Slot " << i << endl; 
					cout << "Remainder " << quotient[i].fr << endl; 
					cout << "Is_occupied: " << quotient[i].is_occupied << endl;
					cout << "Is_continuation: " << quotient[i].is_continuation << endl;
					cout << "Is_shifted: " << quotient[i].is_shifted << endl << endl;
				}
				
			}
			
			macro_stringCast
			bool existeix(paraula p) const{
				cout << endl;
				cout << "Searching paraula " << p << endl;
				bool conte = false;
				unsigned long f = Murmur_hash(p);
				int mod = (int)pow(2.0, r);
				unsigned long remainder = f % mod; //fr
				unsigned long qindex = floor(f / mod); //fq
				
				cout << "remainder es: " << remainder << endl;
				cout << "qindex es: " << qindex << endl;
				
				if (quotient[qindex].is_occupied) {
					if (quotient[qindex].fr == remainder) conte = true;
					else {
						qindex++;
						cout << "SOFT COLLISION" << endl;
						while (quotient[qindex].is_shifted and qindex < m) {
							cout << "qindex(soft) es: " << qindex << endl;
							cout << "remainder(soft) es: " << remainder << endl;
							if (quotient[qindex].fr == remainder){
								conte = true;
								break;
							}
						}
					}
				}
				else {
					cout << "ELEMENTO NO ESTA" << endl;
				}
				cout << endl;
                return conte;
            }
	};

}

#endif
